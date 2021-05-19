/************************************************************************
 * \file IS530_Plastic.hh
 *
 * \class IS530_Plastic   
 * \brief Plastic Scintillator and holding structure  
 *
 * \author R. Lica based on the implementation of C. Sotty
 *************************************************************************/
// It is not implemented as a sensitive detector


#include "IS530_Plastic.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



IS530_Plastic::IS530_Plastic(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin)
  :mother(p_mother), rho(p_rho), theta(p_theta), phi(p_phi), spin(p_spin)
{
	//G4cout	<<"\nIS530_Plastic (Not a sensitive detector) #######################\n"	<<flush
	//	<<"\nRho =\t\t"		<<rho/mm	<< " mm"	<<flush
	//	<<"\nTheta =\t\t"	<<theta/deg	<< " deg" 	<<flush
	//	<<"\nPhi =\t\t"		<<phi/deg	<< " deg"       <<flush
	//	<<"\nSpin =\t\t"	<<spin/deg	<< " deg"       <<flush<<G4endl;
}

IS530_Plastic::~IS530_Plastic()
{;}


G4VPhysicalVolume* IS530_Plastic::Construct()
{
    // Naming
    //
    G4String name="/IS530_Plastic";
    
    // Material
    //
    G4NistManager* NISTmgr = G4NistManager::Instance();

    G4Material* Vacuum          = NISTmgr->FindOrBuildMaterial("G4_Galactic");
    G4Material* Air             = NISTmgr->FindOrBuildMaterial("G4_AIR");
    G4Material* PlastScint 	= NISTmgr->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); 

    
    G4Material* mylar = new G4Material("Mylar",1.365*g/cm3,3); //
    G4Element*  H = new G4Element("Hydrogen" ,  "H", 1 ,   1.01*g/mole); 
    G4Element*  C = new G4Element("Carbone"  ,  "C", 6 ,  12.01*g/mole);
    G4Element*  O = new G4Element("Oxygene"  ,  "O", 8 ,  16.00*g/mole); 
    mylar->AddElement(H, 8);
    mylar->AddElement(C,10);
    mylar->AddElement(O, 4);
    


    vector<G4Material* > Mat;
    Mat.push_back(PlastScint); 
    Mat.push_back(PlastScint);
    Mat.push_back(PlastScint);
    Mat.push_back(mylar);


    // Visualisation
    //
    vector<G4VisAttributes*> VisAttM;
  
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.4)));		//Blue transparent -plastic front
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.4)));		//Blue transparent -plastic back
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.7, 0.4, 0.1)));		//Brown -holding
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.65,0.65,0.65)));		//Grey  -tape

   
     //////////////////////////////////////
  // DETECTOR PLACEMENT

	theta = theta + 180.*deg;
	spin = spin - 90.*deg; 
	rho = rho + 2;
	
		//ROTATION and TRANSLATION
		//Spherical coordinates: rho, theta, phi
		 
      G4RotationMatrix rotation = G4RotationMatrix();
 
      if(0 != spin) rotation.rotateZ(spin);
      rotation.rotateY(phi);
      rotation.rotateX(theta);   
         

      G4ThreeVector translation(0., 0., rho);
      if(0 != spin) translation.rotateZ(spin);
      translation.rotateY(phi);
      translation.rotateX(theta);   
                 
      G4Transform3D transformation(rotation, translation);
   

    
    // Detector Construction
    //
    vector<char* > IS530_Plastic_name;

    IS530_Plastic_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Plastic/Bucharest_Plastic.stl");
    IS530_Plastic_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Plastic/Bucharest_Plastic_back.stl");
    IS530_Plastic_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Plastic/Bucharest_Plastic_holders.stl");
    IS530_Plastic_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Plastic/Tape.stl");
    
    // Meshing and Logical Volumes
    for(int i = 0; i<IS530_Plastic_name.size(); i++){ 
      G4cout<< IS530_Plastic_name.at(i) << G4endl;
      mesh_IS530_Plastic_current.push_back( new CADMesh(IS530_Plastic_name.at(i), mm, G4ThreeVector(0*cm, 0*cm, 0*cm), false));
      //G4cout << "mesh_IS530_Plastic_current" << G4endl;
      IS530_Plastic_current_sol.push_back( mesh_IS530_Plastic_current.at(i)->TessellatedMesh());
      //G4cout << "IS530_Plastic_current_sol" << G4endl;
      IS530_Plastic_current_log.push_back( new G4LogicalVolume(IS530_Plastic_current_sol.at(i), Mat.at(i), name+Form("IS530_Plastic_%i_log", i )));
      //G4cout << "IS530_Plastic_current_log" << G4endl;
      IS530_Plastic_current_log.at(i)	->SetVisAttributes(VisAttM.at(i));
      //G4cout << "SetVisAttributes" << G4endl;
      IS530_Plastic_current_phys.push_back( new G4PVPlacement(transformation, name+Form("/IS530_Plastic_%i_phys",i), IS530_Plastic_current_log.at(i), mother, false, 0));
      //G4cout << "IS530_Plastic_current_phys" << G4endl;
    }

    return(mother);

}






















