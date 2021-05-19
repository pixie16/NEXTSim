#include "TFile.h"
#include "TTree.h"

#include "nDetParticleSource.hh"
#include "nDetParticleSourceMessenger.hh"
#include "nDetDetector.hh"
#include "nDetRunAction.hh"
#include "cmcalc.hh"
#include "termColors.hh"
#include "optionHandler.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4Electron.hh"
#include "Randomize.hh"

const double fwhm2stddev = 1/(2*std::sqrt(2*std::log(2)));

const double coeff = 1.23984193E-3; // hc = Mev * nm
const double cvac = 299.792458; // mm/ns

///////////////////////////////////////////////////////////////////////////////
// class nDetParticleSource
///////////////////////////////////////////////////////////////////////////////

nDetParticleSource &nDetParticleSource::getInstance(){
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static nDetParticleSource instance;
	return instance;
}

nDetParticleSource::nDetParticleSource(nDetDetector *det/*=NULL*/) : G4GeneralParticleSource(), fSourceMessenger(NULL), unitX(1,0,0), unitY(0,1,0), unitZ(0,0,1),
                                                                     sourceOrigin(0,0,0), beamspotType(0), beamspot(0), beamspot0(0), rot(), targThickness(0),targEnergyLoss(0),
                                                                     targTimeSlope(0), targTimeOffset(0), beamE0(0), useReaction(false), isotropic(false), back2back(false), realIsotropic(false),
                                                                     particleRxn(NULL), detPos(), detSize(), detRot(), sourceIndex(0), numSources(0), interpolationMethod("Lin")
{
	// Set the default particle source.
	SetNeutronBeam(1.0); // Set a 1 MeV neutron beam by default
	
	// Set the default beam direction along the +X axis (to be changed to +Z later CRT)
	SetSourceDirection(G4ThreeVector(0, 0, 0));
	
	// Set the default particle reaction.
	particleRxn = new Reaction();

	if(det)
		this->SetDetector(det);
	
	// Create a messenger for this class
	fSourceMessenger = new nDetParticleSourceMessenger(this); 
}

nDetParticleSource::~nDetParticleSource(){ 
	delete particleRxn;
}

void nDetParticleSource::SetBeamEnergy(const G4double &energy){
		GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
		GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
}

bool nDetParticleSource::SetBeamEnergySigma(const G4String &str){
	// Expects a space-delimited string of the form:
	//  "E(MeV) dE(MeV)"
	std::vector<std::string> args;
	unsigned int Nargs = split_str(str, args);
	if(Nargs < 2){
		std::cout << " nDetConstruction: Invalid number of arguments given to ::SetBeamEnergySigma(). Expected 2, received " << Nargs << ".\n";
		std::cout << " nDetConstruction:  SYNTAX: <E> <dE>\n";
		return false;
	}
	double energy = strtod(args.at(0).c_str(), NULL);
	double sigma = strtod(args.at(1).c_str(), NULL);
	SetBeamEnergySigma(energy, sigma);
	return true;
}

void nDetParticleSource::SetBeamEnergySigma(const G4double &energy, const G4double &dE){
	GetCurrentSource()->GetEneDist()->SetEnergyDisType("Gauss");
	GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
	GetCurrentSource()->GetEneDist()->SetBeamSigmaInE(dE);
}

void nDetParticleSource::SetSourcePosition(const G4ThreeVector &position){
	G4SingleParticleSource *src;
	while( (src = nextSource()) ){ // Iterate over all defined sources
		src->GetPosDist()->SetCentreCoords(position);
	}
}

void nDetParticleSource::SetSourceDirection(const G4ThreeVector &d){ 
	// Reset the unit vectors and the source rotation matrix
	unitX = G4ThreeVector(1, 0, 0);
	unitY = G4ThreeVector(0, 1, 0);
	unitZ = G4ThreeVector(0, 0, 1);
	rot = G4RotationMatrix();

	// Rotate the source into the new frame
	rot.rotateX(d.getX()*deg);
	rot.rotateY(d.getY()*deg);
	rot.rotateZ(d.getZ()*deg);
	unitX = rot*unitX;
	unitY = rot*unitY;
	unitZ = rot*unitZ;
	
	/*std::cout << " debug: x=(" << unitX.getX() << ", " << unitX.getY() << ", " << unitX.getZ() << ")\n";
	std::cout << " debug: y=(" << unitY.getX() << ", " << unitY.getY() << ", " << unitY.getZ() << ")\n";
	std::cout << " debug: z=(" << unitZ.getX() << ", " << unitZ.getY() << ", " << unitZ.getZ() << ")\n";*/
}

bool nDetParticleSource::SetSourceType(const G4String &str){
	// Expects a space-delimited string of the form:
	//  "<name> [energy(MeV)]"
	std::vector<std::string> args;
	unsigned int Nargs = split_str(str, args);
	if(Nargs < 1){
		Display::ErrorPrint("Invalid number of arguments given to ::SetSourceType().", "nDetParticleSource");
		Display::ErrorPrint(" SYNTAX: type <name> [energy(MeV)]", "nDetParticleSource");
		return false;
	}

	std::string typeName = args.at(0);
	double beamEnergy = 1;
	if(Nargs >= 2)
		beamEnergy = strtod(args.at(1).c_str(), NULL);

	// Set the type of source or beam
	if(typeName == "137Cs")
		Set137Cs();
	else if(typeName == "60Co")
		Set60Co();
	else if(typeName == "133Ba")
		Set133Ba();
	else if(typeName == "241Am")
		Set241Am();
	else if(typeName == "90Sr")
		Set90Sr();
	else if(typeName == "252Cf")
		Set252Cf();
	else if(typeName == "neutron")
		SetNeutronBeam(beamEnergy);
	else if(typeName == "gamma")
		SetGammaRayBeam(beamEnergy);
	else if(typeName == "laser"){
		if(beamEnergy == 1) // Default value
			beamEnergy = 420; // nm
		beamEnergy = coeff / beamEnergy; // Now in MeV
		SetLaserBeam(beamEnergy);
	}
	else if(typeName == "electron")
		SetElectronBeam(beamEnergy);		
	else{
		Display::ErrorPrint("User specified unknown source type.", "nDetParticleSource");
		return false;
	}
	std::cout << " nDetParticleSource: Setting " << typeName << " source.\n";
	return true;
}

void nDetParticleSource::SetParticleMomentumDirection(const G4ParticleMomentum &direction){
	G4SingleParticleSource *src;
	while( (src = nextSource()) ){ // Iterate over all defined sources
		src->GetAngDist()->SetParticleMomentumDirection(direction);
	}
}

void nDetParticleSource::SetBeamspotType(const G4String &str){
	if(str == "point")
		beamspotType = 0;
	else if(str == "circle")
		beamspotType = 1;
	else if(str == "annulus")
		beamspotType = 2;
	else if(str == "ellipse")
		beamspotType = 3;
	else if(str == "square")
		beamspotType = 4;
	else if(str == "rectangle")
		beamspotType = 5;
	else if(str == "vertical")
		beamspotType = 6;
	else if(str == "horizontal")
		beamspotType = 7;
	else if(str == "gauss")
		beamspotType = 8;
	else{
		Display::ErrorPrint("Unknown beamspot type.", "nDetParticleSource");
		beamspotType = 0;
		return;
	}
	std::cout << " nDetParticleSource: Setting beam profile to type " << beamspotType << " (" << str << ")\n";
}

void nDetParticleSource::SetEnergyLimits(const double &Elow_, const double &Ehigh_){
	std::cout << " nDetParticleSource: Setting energy distribution sampling in the range " << Elow_ << " MeV to " << Ehigh_ << " MeV.\n";
	GetCurrentSource()->GetEneDist()->SetEmin(Elow_*MeV);
	GetCurrentSource()->GetEneDist()->SetEmax(Ehigh_*MeV);
}

void nDetParticleSource::SetDetector(const nDetDetector *det){
	detPos = det->GetDetectorPos();
	detSize = det->GetDetectorSize();
	detRot = det->GetDetectorRot();
}

void nDetParticleSource::Set252Cf(const size_t &size_/*=150*/, const double &stepSize_/*=0.1*/){
	Reset(); // Should this always clear the source? CRT
	
	// Set to a neutron
	GetCurrentSource()->SetParticleDefinition(G4Neutron::NeutronDefinition());

	G4SPSEneDistribution *ene = GetCurrentSource()->GetEneDist();
	ene->SetEnergyDisType("Arb");

	// First point is the low edge of the first energy bin
	G4ThreeVector point(0, cf252(0), 0);
	ene->ArbEnergyHisto(point);
	
	for(size_t i = 1; i < size_+1; i++){
		point.setX(i*stepSize_*MeV);
		point.setY(cf252(point.x()));
		ene->ArbEnergyHisto(point);
	}
	ene->ArbInterpolate(interpolationMethod);
}

void nDetParticleSource::Set137Cs(){
	Reset();
	AddDiscreteEnergy(4.47, 0.91);
	AddDiscreteEnergy(31.817, 1.99);
	AddDiscreteEnergy(31.817, 1.99);
	AddDiscreteEnergy(32.194, 3.64);
	AddDiscreteEnergy(36.304, 0.348);
	AddDiscreteEnergy(36.378, 0.672);
	AddDiscreteEnergy(37.255, 0.213);
	AddDiscreteEnergy(661.657, 85.10);
}

void nDetParticleSource::Set60Co(){
	Reset();
	AddDiscreteEnergy(7.461, 0.00322);
	AddDiscreteEnergy(7.478, 0.0063);
	AddDiscreteEnergy(347.14, 0.0075);
	AddDiscreteEnergy(826.10, 0.0076);
	AddDiscreteEnergy(1173.228, 99.85, true);
	AddDiscreteEnergy(1332.492, 99.9826, true);
	AddDiscreteEnergy(2158.57, 0.00120);
}

void nDetParticleSource::Set133Ba(){
	Reset();
	AddDiscreteEnergy(4.29, 15.7);
	AddDiscreteEnergy(30.625, 33.9);
	AddDiscreteEnergy(30.973, 62.2);
	AddDiscreteEnergy(34.92, 5.88);
	AddDiscreteEnergy(34.987, 11.4);
	AddDiscreteEnergy(35.818, 3.51);
	AddDiscreteEnergy(53.1622, 2.14);
	AddDiscreteEnergy(79.6142, 2.65);
	AddDiscreteEnergy(80.9979, 32.9);
	AddDiscreteEnergy(160.612, 0.638);
	AddDiscreteEnergy(223.2368, 0.453);
	AddDiscreteEnergy(276.3989, 7.16);
	AddDiscreteEnergy(302.8508, 18.34);
	AddDiscreteEnergy(356.0129, 62.05);
	AddDiscreteEnergy(383.8485, 8.94);
}

void nDetParticleSource::Set241Am(){
	Reset();
	AddDiscreteEnergy(13.9, 37);
	AddDiscreteEnergy(26.3446, 2.27);
	AddDiscreteEnergy(33.196, 0.126);
	AddDiscreteEnergy(42.704, 0.0055);
	AddDiscreteEnergy(43.42, 0.073);
	AddDiscreteEnergy(55.56, 0.0181);
	AddDiscreteEnergy(59.5409, 35.9);
	AddDiscreteEnergy(69.76, 0.0029);
	AddDiscreteEnergy(97.069, 0.00114);
	AddDiscreteEnergy(98.97, 0.0203);
	AddDiscreteEnergy(101.059, 0.00181);
	AddDiscreteEnergy(102.98, 0.0195);
}

void nDetParticleSource::Set90Sr(){
	Reset();
	AddDiscreteEnergy(195.8, 100, false, G4Electron::ElectronDefinition());
}

void nDetParticleSource::SetNeutronBeam(const double &energy_){
	Reset();
	GetCurrentSource()->SetParticleDefinition(G4Neutron::NeutronDefinition());
	SetBeamEnergy(energy_*MeV);
}

void nDetParticleSource::SetGammaRayBeam(const double &energy_){
	Reset();
	GetCurrentSource()->SetParticleDefinition(G4Gamma::GammaDefinition());
	SetBeamEnergy(energy_*MeV);
}

void nDetParticleSource::SetLaserBeam(const double &energy_){
	Reset();
	GetCurrentSource()->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
	SetBeamEnergy(energy_*MeV);
}

void nDetParticleSource::SetElectronBeam(const double &energy_){
	Reset();
	GetCurrentSource()->SetParticleDefinition(G4Electron::ElectronDefinition());
	SetBeamEnergy(energy_*MeV);
}

bool nDetParticleSource::ReadEnergyFile(const char *filename){
	Reset();
	
	std::ifstream ifile(filename);
	if(!ifile.good()){
		Display::ErrorPrint("Failed to open input energy distribution file.", "nDetParticleSource");
		return false;
	}

	// Set to a neutron
	GetCurrentSource()->SetParticleDefinition(G4Neutron::NeutronDefinition());  //hard coded for now - JNH
	G4SPSEneDistribution *ene = GetCurrentSource()->GetEneDist();
	ene->SetEnergyDisType("Arb");
	
	double energy, val;
	G4ThreeVector point;
	while(true){
		ifile >> energy >> val;
		if(ifile.eof()) break;
		point.setX(energy*MeV);
		point.setY(val);
		ene->ArbEnergyHisto(point);
	}
	ene->ArbInterpolate(interpolationMethod);
	
	ifile.close();
	return true;
}

bool nDetParticleSource::ReadReactionFile(const G4String &input){
	std::vector<std::string> args;
	unsigned int Nargs = split_str(input, args);

	std::string fname = input;
	if(Nargs > 1){
		if(Nargs < 3){
			Display::ErrorPrint("Invalid number of arguments given to ::LoadReactionFile().", "nDetParticleSource");
			Display::ErrorPrint(" SYNTAX: reaction <filename> [<thickness> <dE/dx>]", "nDetParticleSource");
			return false;
		}
		fname = args.front();
		targThickness = strtod(args.at(1).c_str(), NULL)*mm;
		targEnergyLoss = strtod(args.at(2).c_str(), NULL)*MeV/mm;
	}
	else{
		beamE0 = 0;
		targThickness = 0;
		targEnergyLoss = 0;
	}
	targTimeSlope = 0;

	std::cout << " nDetParticleSource: Loading reaction parameters from file \"" << fname << "\"...\n";
	bool retval = particleRxn->Read(fname.c_str());
	if(retval){
		useReaction = true;
		if(Nargs > 1){
			beamE0 = particleRxn->GetBeamEnergy();
			std::cout << " nDetParticleSource: Set target thickness to " << targThickness << " mm and projectile (dE/dx) to " << targEnergyLoss << " MeV/mm.\n";
		
			// Compute the target time-offset slope.
			double partMass = particleRxn->GetEjectileMass()/(cvac*cvac);
			double xStep = targThickness/100;
			double currentEnergy = beamE0;
			for(int i = 0; i < 100; i++){
				targTimeSlope += xStep*std::sqrt(partMass/(2*currentEnergy));
				currentEnergy += xStep*targEnergyLoss;
			}
			targTimeSlope = targTimeSlope/targThickness;
		}
	}
	else{
		useReaction = false;
	}
	
	Reset(); // Reset the source if the reaction was loaded successfully
	GetCurrentSource()->SetParticleDefinition(G4Neutron::NeutronDefinition()); // Hard-coded neutrons for now CRT
	
	return retval;
}

bool nDetParticleSource::AddDiscreteEnergy(const G4String &str){
	// Expects a space-delimited string of the form:
	//  "<energy(keV)> [intensity] [particle]"
	std::vector<std::string> args;
	unsigned int Nargs = split_str(str, args);
	if(Nargs < 1){
		Display::ErrorPrint("Invalid number of arguments given to ::AddDiscreteEnergy().", "nDetParticleSource");
		Display::ErrorPrint(" SYNTAX: addLevel <energy(keV)> [intensity=100] [particle=gamma]", "nDetParticleSource");
		return false;
	}
	double energy = strtod(args.at(0).c_str(), NULL);
	double intensity = 100;
	if(Nargs >= 2) // Set the level intensity
		intensity = strtod(args.at(1).c_str(), NULL);
	G4ParticleDefinition *particle = NULL;
	if(Nargs >= 3){ // Set the particle definition
		if(args.at(2) == "neutron")
			particle = G4Neutron::NeutronDefinition();
		else if(args.at(2) == "gamma")
			particle = G4Gamma::GammaDefinition();
		else if(args.at(2) == "electron")
			particle = G4Electron::ElectronDefinition();
		else{
			Display::ErrorPrint("User specified particle type that is not known to NEXTSim", "nDetParticleSource");
			return false;
		}
	}
	AddDiscreteEnergy(energy, intensity, false, particle);
	return true;
}

void nDetParticleSource::AddDiscreteEnergy(const G4double &energy, const G4double &intensity, const bool &isPair/*=false*/, G4ParticleDefinition *particle/*=NULL*/){
	if(numSources++ > 1)
		addNewSource();
	GetCurrentSource()->SetParticleDefinition((particle ? particle : G4Gamma::GammaDefinition()));
	SetBeamEnergy(energy*keV);
	SetCurrentSourceIntensity(intensity/100);
	if(isPair){ // Only pairs of energy levels supported for now
		if(b2bEnergy[0] < 0)
			b2bEnergy[0] = energy*keV;
		else if(b2bEnergy[1] < 0)
			b2bEnergy[1] = energy*keV;
		back2back = true;
	}
}

void nDetParticleSource::Reset(){
	ClearAll();
	allSources.clear();
	addNewSource();
	sourceIndex = 0;
	numSources = 1;
	back2back = false;
	b2bEnergy[0] = -1;
	b2bEnergy[1] = -1;
}

void nDetParticleSource::UpdateAll(){
	G4SingleParticleSource *src;
	while( (src = nextSource()) ){ // Iterate over all defined sources
		// Set the center of the source
		src->GetPosDist()->SetCentreCoords(sourceOrigin);

		// Set the profile of the source
		setBeamProfile(src);

		// For now, the "beam" is along the +X axis
		// I'll change this  back to the +Z axis later. CRT	
		src->GetAngDist()->SetParticleMomentumDirection(unitX);
	
		// Set the rotation of the source plane
		src->GetPosDist()->SetPosRot1(unitZ); // This is x'
		src->GetPosDist()->SetPosRot2(unitY); // This is y'
	}
}

bool nDetParticleSource::Test(const G4String &str){
	// Expects a space-delimited string of the form:
	//  "<filename> <Nevents>"
	std::vector<std::string> args;
	unsigned int Nargs = split_str(str, args);
	if(Nargs < 2){
		Display::ErrorPrint("Invalid number of arguments given to ::Test().", "nDetParticleSource");
		Display::ErrorPrint(" SYNTAX: testSource <filename> <Nevents>", "nDetParticleSource");
		return false;
	}
	return Test(args.at(0).c_str(), strtoul(args.at(1).c_str(), NULL, 10));
}

bool nDetParticleSource::Test(const char *filename, const size_t &Nevents){
	double energy;
	double pos[3];
	double dir[3];
	double theta;
	TFile *f = new TFile(filename, "RECREATE");
	if(!f->IsOpen()){
		Display::ErrorPrint("Failed to open output ROOT file.", "nDetParticleSource");
		return false;
	}
	TTree *tree = new TTree("data", "Energy dist test");
	tree->Branch("energy", &energy);
	tree->Branch("theta", &theta);
	tree->Branch("pos[3]", pos);
	tree->Branch("dir[3]", dir);
	UpdateAll(); // Update the source with the most recent changes
	for(size_t i = 0; i < Nevents; i++){
		G4Event dummyEvent;
		energy = Sample(&dummyEvent);
		GeneratePrimaries(&dummyEvent);
		G4PrimaryVertex *vertex = dummyEvent.GetPrimaryVertex(0);
		G4ThreeVector position = vertex->GetPosition();
		G4ThreeVector direction = vertex->GetPrimary()->GetMomentumDirection();	
		energy = vertex->GetPrimary()->GetKineticEnergy();
		theta = std::acos(unitX.dot(direction))*180/3.14159;
		pos[0] = position.x();
		pos[1] = position.y();
		pos[2] = position.z();
		dir[0] = direction.x();
		dir[1] = direction.y();
		dir[2] = direction.z();
		tree->Fill();
	}
	std::cout << " nDetParticleSource: Successfully wrote " << Nevents << " to " << filename << std::endl;
	f->cd();
	tree->Write();
	f->Close();
	delete f;
	return true;
}

double nDetParticleSource::Sample(G4Event *evt/*=NULL*/){
	double retval;
	if(evt == NULL){
		G4Event dummyEvent;
		GeneratePrimaryVertex(&dummyEvent);
		retval = dummyEvent.GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy();
	}
	else{
		GeneratePrimaryVertex(evt);
		retval = evt->GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy();
	}
	return retval;
}

double nDetParticleSource::Print(const size_t &Nsamples/*=1*/){
	double retval = 0;
	if(!useReaction){
		for(size_t i = 0; i < Nsamples; i++){
			retval = Sample();
			if(Nsamples == 1)
				std::cout << " energy=" << retval << " MeV\n";
			else
				std::cout << retval << std::endl;
		}
	}
	else{
		double theta;
		for(size_t i = 0; i < Nsamples; i++){
			theta = G4UniformRand()*180;
			retval = particleRxn->sample(theta);
			if(Nsamples == 1)
				std::cout << " theta=" << theta << ", energy=" << retval << " MeV\n";
			else
				std::cout << theta << "\t" << retval << std::endl;
		}
	}
	return retval;
}

void nDetParticleSource::generateIsotropic(G4PrimaryVertex *vertex){
	G4PrimaryParticle *particle = vertex->GetPrimary();
	G4ThreeVector direction = particle->GetMomentumDirection();	

	double theta;
	G4ThreeVector vRxnDet, dirPrime;
	if(useReaction && targThickness > 0){ // Use psuedo-realistic target energy loss.
		double reactionDepth = targThickness*G4UniformRand();
		G4ThreeVector reactionPoint = vertex->GetPosition()+(-targThickness/2+reactionDepth)*direction;
		vRxnDet = detPos - reactionPoint;
		vertex->SetPosition(reactionPoint.x(), reactionPoint.y(), reactionPoint.z()); // Set the reaction point inside the target.
		particleRxn->SetEbeam(beamE0-targEnergyLoss*reactionDepth); // Set the energy of the projectile at the time of the reaction.
		targTimeOffset = targTimeSlope*(reactionDepth - 0.5*targThickness); // Compute the global time offset due to the target.
	}
	else{
		vRxnDet = detPos;
	}
	
	if(!realIsotropic){ // Generate a psuedo-isotropic event because true isotropic would be much slower
		// Generate a random point inside the volume of the detector in the frame of the detector
		// Get the vector from the center of the detector to a uniformly sampled point inside its volume
		G4ThreeVector insideDet((detSize.getX()/2)*(2*G4UniformRand()-1), (detSize.getY()/2)*(2*G4UniformRand()-1), (detSize.getZ()/2)*(2*G4UniformRand()-1));

		// Transform to the frame of the detector.
		insideDet *= detRot;

		// Compute the direction of the particle emitted from the source.
		dirPrime = vRxnDet + insideDet;
		dirPrime *= (1/dirPrime.mag());
		particle->SetMomentumDirection(dirPrime);
	}
	else{ // Generate a true isotropic event
		double phi;
		unitSphereRandom(theta, phi);
		
		// Convert to cartesian coordinates and
		particle->SetMomentumDirection(sphere2cart(theta, phi));
	}
	
	// Compute the particle energy.
	if(useReaction){
		double energy;
		if(!realIsotropic){
			theta = std::acos(direction.dot(dirPrime));
			energy = particleRxn->sample(theta);
		}
		else{
			energy = particleRxn->sample(theta, false);
		}
		particle->SetKineticEnergy(energy);
	}	
}

void nDetParticleSource::unitSphereRandom(double &theta, double &phi){
	phi = 2*pi*G4UniformRand();
	theta = std::acos(2*G4UniformRand()-1);
}

G4ThreeVector nDetParticleSource::sphere2cart(const double &theta, const double &phi){
	return G4ThreeVector(std::sin(theta)*std::cos(phi), std::sin(theta)*std::sin(phi), std::cos(theta));
}

void nDetParticleSource::GeneratePrimaries(G4Event* anEvent){
	// Enable the mutex lock to protect access
	generatorLock.lock();
	
	GeneratePrimaryVertex(anEvent);
	if(useReaction || isotropic) // Generate particles psuedo-isotropically
		generateIsotropic(anEvent->GetPrimaryVertex(0));

	if(back2back){ // Generate another back-to-back particle (e.g. 60Co)
		GeneratePrimaryVertex(anEvent);
		G4PrimaryParticle *particle = anEvent->GetPrimaryVertex(1)->GetPrimary();
		particle->SetMomentumDirection(-anEvent->GetPrimaryVertex(0)->GetPrimary()->GetMomentumDirection());
		double kE = anEvent->GetPrimaryVertex(0)->GetPrimary()->GetKineticEnergy();
		if(kE == b2bEnergy[0])
			particle->SetKineticEnergy(b2bEnergy[1]);
		else if(kE == b2bEnergy[1])
			particle->SetKineticEnergy(b2bEnergy[0]);
	}
	
	// Disable the mutex lock to open access
	generatorLock.unlock();
}

G4SingleParticleSource *nDetParticleSource::nextSource(){
	if(sourceIndex >= allSources.size()){ // Reset the counter
		sourceIndex = 0;
		return NULL;
	}
	return allSources.at(sourceIndex++);
}

G4SingleParticleSource *nDetParticleSource::addNewSource(const G4double &intensity/*=1*/){
	AddaSource(intensity);
	G4SingleParticleSource *retval = GetCurrentSource();
	allSources.push_back(retval);
	return retval;	
}

double nDetParticleSource::cf252(const double &E_) const {
	const double a = 1.174; // MeV (Mannhart)
	const double b = 1.043; // 1/MeV (Mannhart)
	const double C = (2/std::sqrt(pi*b*a*a*a))*std::exp(-a*b/4);
	return C*std::exp(-E_/a)*std::sinh(std::sqrt(b*E_));
}

void nDetParticleSource::setBeamProfile(G4SingleParticleSource *src){
	G4SPSPosDistribution *pos = src->GetPosDist();
	if(beamspotType == 0){ // Point source
		pos->SetPosDisType("Point");
	}
	else if(beamspotType == 1){ // Circular profile
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Circle");
		pos->SetRadius(beamspot);
	}
	else if(beamspotType == 2){ // Annular profile
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Annulus");
		pos->SetRadius(beamspot);
		pos->SetRadius0(beamspot0);
	}
	else if(beamspotType == 3){ // Elliptical profile
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Ellipse");
		pos->SetHalfX(beamspot);
		pos->SetHalfY(beamspot0);
	}	
	else if(beamspotType == 4){ // Square profile
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Square");
		pos->SetHalfX(beamspot);
		pos->SetHalfY(beamspot);
	}
	else if(beamspotType == 5){ // Rectangular profile
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Rectangle");
		pos->SetHalfX(beamspot);
		pos->SetHalfY(beamspot0);
	}
	else if(beamspotType == 6){ // Vertical line
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Rectangle");
		pos->SetHalfX(0);
		pos->SetHalfY(beamspot);
	}
	else if(beamspotType == 7){ // Horizontal line
		pos->SetPosDisType("Plane");
		pos->SetPosDisShape("Rectangle");
		pos->SetHalfX(beamspot);
		pos->SetHalfY(0);
	}
	else if(beamspotType == 8){ // 2d gaussian (beamspot=FWHM)
		pos->SetPosDisType("Beam");
		pos->SetBeamSigmaInX(beamspot*fwhm2stddev);
		pos->SetBeamSigmaInY(beamspot0*fwhm2stddev);
	}
}

///////////////////////////////////////////////////////////////////////////////
// class nDetPrimaryGeneratorAction
///////////////////////////////////////////////////////////////////////////////

nDetPrimaryGeneratorAction::nDetPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction() { 
	source = &nDetParticleSource::getInstance();
}

void nDetPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
	// Generate a primary particle (mutex protected)
	source->GeneratePrimaries(anEvent);
}
