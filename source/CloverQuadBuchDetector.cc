/************************************************************************
 * \file CloverQuadBuchDetector.hh
 *
 * \class CloverQuadBuchDetector   
 * \brief Design of a HPGe detector crystal (Clover 40x50x70) (Bucharest)
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/
//R. Lica on 01.03.2016 - Fixed the G4Transform3D bug, removed det_env


#include "CloverSingleBuchDetector.hh"
#include "CloverSingleBuchHit.hh"
#include "CloverQuadBuchDetector.hh"
#include "CloverSingleBuchSD.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



CloverQuadBuchDetector::CloverQuadBuchDetector(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin, G4int p_cl_nb)
  :mother(p_mother), rho(p_rho), theta(p_theta), phi(p_phi), spin(p_spin), cl_nb(p_cl_nb)
{
	//G4cout	<<"\nClover Quad (Bucharest) DETECTOR#######################\n"	<<flush
	//	<<"\nRho =\t\t"		<<rho/mm	<< " mm"	<<flush
	//	<<"\nTheta =\t\t"	<<theta/deg	<< " deg" 	<<flush
	//	<<"\nPhi =\t\t"		<<phi/deg	<< " deg"       <<flush
	//	<<"\nSpin =\t\t"	<<spin/deg	<< " deg"       <<flush
	//	<<"\nCloverNb =\t"	<<cl_nb 			<<G4endl;
}

CloverQuadBuchDetector::~CloverQuadBuchDetector()
{;}


G4VPhysicalVolume* CloverQuadBuchDetector::Construct()
{
    // Naming
    //
    G4String name="/CloverBuch"+cl_nb;
    
    // Material
    //
    G4NistManager* NISTmgr = G4NistManager::Instance();

    G4Material* Vacuum          = NISTmgr->FindOrBuildMaterial("G4_Galactic");
    G4Material* Air             = NISTmgr->FindOrBuildMaterial("G4_AIR");
    G4Material* Al_mat          = NISTmgr->FindOrBuildMaterial("G4_Al");

    // Visualisation
    // 
    vector<G4VisAttributes*> CrysVisAttM;
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,0.0,1.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(1.0,0.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,1.0)));

    // Visualisation
    //
    Crystal0_vis_att = new G4VisAttributes(G4Colour(0.0,0.0,1.0));   
    Crystal1_vis_att = new G4VisAttributes(G4Colour(0.0,1.0,0.0));   
    Crystal2_vis_att = new G4VisAttributes(G4Colour(1.0,0.0,0.0));   
    Crystal3_vis_att = new G4VisAttributes(G4Colour(0.0,1.0,1.0));   
    G4VisAttributes* Al_vis_att 	= new G4VisAttributes(G4Colour(0.65,0.65,0.65));
    Capsule_vis_att       = Al_vis_att;    


    // Detector Construction
    //
    // Meshing
    mesh_Capsule	= new CADMesh(const_cast<char*>("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Clover_Bucharest_RefFace/Clover_Assembly_Bucharest_RefModif_Capsule_Bucharest_redesigned_1.stl"),       mm,  G4ThreeVector( 0*cm, 0*cm, 0*cm), false);
    Capsule_sol		= mesh_Capsule->TessellatedMesh();

    // Logical Volume
    Capsule_log		= new G4LogicalVolume(Capsule_sol       , Al_mat, name+"/Capsule_log"           );
    Capsule_log		->SetVisAttributes(Capsule_vis_att);
    

    // Position and Orientation
    rho = -rho ;

    G4RotationMatrix rotation = G4RotationMatrix();
 
      if(0 != spin) rotation.rotateZ(spin);
      rotation.rotateY(phi);
      rotation.rotateX(theta);   

    
      G4ThreeVector translation(0., 0., rho);
      if(0 != spin) translation.rotateZ(spin);
      translation.rotateY(phi);
      translation.rotateX(theta);   
      
      G4Transform3D transformation(rotation, translation);

     
    // Capsule
    Capsule_phys       = new G4PVPlacement(transformation, name+"/Capsule", Capsule_log, mother, false, 0);
    
    // Crystals
    Crystal0	= new CloverSingleBuchDetector(mother, transformation,  cl_nb, 0, Crystal0_vis_att);
    Crystal1	= new CloverSingleBuchDetector(mother, transformation,  cl_nb, 1, Crystal1_vis_att);
    Crystal2	= new CloverSingleBuchDetector(mother, transformation,  cl_nb, 2, Crystal2_vis_att);
    Crystal3	= new CloverSingleBuchDetector(mother, transformation,  cl_nb, 3, Crystal3_vis_att);

    G4VPhysicalVolume* cl0 = Crystal0->Construct();
    G4VPhysicalVolume* cl1 = Crystal1->Construct();
    G4VPhysicalVolume* cl2 = Crystal2->Construct();
    G4VPhysicalVolume* cl3 = Crystal3->Construct();


    return(mother);

}






















