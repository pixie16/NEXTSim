/************************************************************************
 * \file Tape.hh
 *
 * \class Tape   
 * \brief Design of the Tape frame holding the HPGe detectors
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/

#include "Tape.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



Tape::Tape(G4VPhysicalVolume* p_mother)
  :mother(p_mother)
{
	//G4cout	<<"\nTape  #######################\n"	<<flush;
}

Tape::~Tape()
{;}


G4VPhysicalVolume* Tape::Construct()
{
    // Naming
    //
    G4String name="/Tape";
    
    // Material
    //
    G4NistManager* NISTmgr = G4NistManager::Instance();

    G4Material* Vacuum          = NISTmgr->FindOrBuildMaterial("G4_Galactic");
    G4Material* Air             = NISTmgr->FindOrBuildMaterial("G4_AIR");
    G4Material* Al_mat          = NISTmgr->FindOrBuildMaterial("G4_Al");

    G4Element*  H = new G4Element("Hydrogen" ,  "H", 1 ,   1.01*g/mole); 
    G4Element*  C = new G4Element("Carbone"  ,  "C", 6 ,  12.01*g/mole);
    G4Element*  O = new G4Element("Oxygene"  ,  "O", 8 ,  16.00*g/mole);
            
    G4Material* mylar = new G4Material("Mylar",1.365*g/cm3,3); //
    mylar->AddElement(H, 8);
    mylar->AddElement(C,10);
    mylar->AddElement(O, 4);


    // Visualisation
    //
    G4VisAttributes* Al_vis_att 	= new G4VisAttributes(G4Colour(0.65,0.65,0.65));

    // Detector Construction
    //
    G4double tape_thickness = 0.15; // in mm

    G4Box* Tape_imp	= new G4Box("Tape_imp", 200.*mm, 12.5/2.*mm, tape_thickness/2.0*mm);// Implantation Tape (mylar)
    Tape_imp_log	= new G4LogicalVolume(Tape_imp,mylar, name+"/Tape_imp_log");
    Tape_imp_phys	= new G4PVPlacement(0,     G4ThreeVector( 0, 0, (-1)*tape_thickness/2*mm), name+"/Tape_imp_phys", Tape_imp_log, mother, false, 0);

    G4Box* Tape_back 	= new G4Box("Tape_back", 200.*mm, 12.5/2.*mm,tape_thickness/2.0*mm);// Implantation Tape (mylar)
    Tape_back_log 	= new G4LogicalVolume(Tape_imp,mylar, name+"/Tape_back_log");
    Tape_back_phys	= new G4PVPlacement(0, G4ThreeVector( 0, 0, ((-1)*tape_thickness/2 + 1.5)*mm), name+"/Tape_back_phys", Tape_back_log, mother, false, 0);

    return(mother);

}




