/************************************************************************
 * \file IS530_Chamber.hh
 *
 * \class IS530_Chamber   
 * \brief Design of the IS530_Chamber (	Versatile Implantation Chamber
 * 					for the Fast Timing and
 *                              	Standard beta-decay configurations) 
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/

#include "IS530_Chamber.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



IS530_Chamber::IS530_Chamber(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin)
  :mother(p_mother), rho(p_rho), theta(p_theta), phi(p_phi), spin(p_spin)
{
	//G4cout	<<"\nIS530_Chamber (Implantation Chamber) #######################\n"	<<flush
	//	<<"\nRho =\t\t"		<<rho/mm	<< " mm"	<<flush
	//	<<"\nTheta =\t\t"	<<theta/deg	<< " deg" 	<<flush
	//	<<"\nPhi =\t\t"		<<phi/deg	<< " deg"       <<flush
	//	<<"\nSpin =\t\t"	<<spin/deg	<< " deg"       <<flush<<G4endl;
}

IS530_Chamber::~IS530_Chamber()
{;}


G4VPhysicalVolume* IS530_Chamber::Construct()
{
    // Naming
    //
    G4String name="/IS530_Chamber";
    
    // Material
    //
    G4NistManager* NISTmgr = G4NistManager::Instance();

    G4Material* Vacuum          = NISTmgr->FindOrBuildMaterial("G4_Galactic");
    G4Material* Air             = NISTmgr->FindOrBuildMaterial("G4_AIR");
    G4Material* Al_mat          = NISTmgr->FindOrBuildMaterial("G4_Al");
    

    vector<G4Material* > Mat;
    Mat.push_back(Al_mat);
    Mat.push_back(Al_mat);


    // Visualisation
    // 
    vector<G4VisAttributes*> CrysVisAttM;
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,0.0,1.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(1.0,0.0,0.0)));
    CrysVisAttM.push_back(new G4VisAttributes(G4Colour(0.0,1.0,1.0)));

    // Visualisation
    //
    vector<G4VisAttributes*> VisAttM;
    IS530_Chamber_vis_att			= new G4VisAttributes(G4Colour(0.7, 0.4, 0.1));		// Brown
    G4VisAttributes* Al_vis_att 	= new G4VisAttributes(G4Colour(0.65,0.65,0.65));	// Grey
    VisAttM.push_back(new G4VisAttributes(G4Colour(0.7, 0.4, 0.1)));		// Brown
    VisAttM.back()->SetForceSolid(true);
    VisAttM.push_back(new G4VisAttributes(G4Colour(1.0, 1.0, 0.0,0.1)));		// Yellow


     //////////////////////////////////////
  // DETECTOR PLACEMENT

	  theta = theta + 180.*deg;
	
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

    // Environnement
    G4double detsize_y	= 450.*mm;
    G4double detsize_z	= 450.*mm;
    G4double detsize_x	= 450.*mm;
    G4Box* det_env_sol	= new G4Box(name+"/det_env", detsize_x, detsize_y, detsize_z);
    //det_env_log		= new G4LogicalVolume(det_env_sol, Vacuum, name+"/det_env_log");
    //det_env_log		-> SetVisAttributes(G4VisAttributes::Invisible);
    //det_env		= new G4PVPlacement(*IS530_Chamber_transformation, name+"/env", det_env_log, mother, false, 0);
    

    // Detector Construction
    //
    vector<char* > IS530_Chamber_name;
    IS530_Chamber_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Chamber/Bucharest_Chamber_IS530_capac_leon.stl");
    IS530_Chamber_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/Bucharest_Chamber/Bucharest_Chamber_IS530_incinta_leon.stl");

    // Meshing and Logical Volumes
    for(int i = 0; i<IS530_Chamber_name.size(); i++){ 
      G4cout<< IS530_Chamber_name.at(i) << G4endl;
      mesh_IS530_Chamber_current.push_back( new CADMesh(IS530_Chamber_name.at(i), mm, G4ThreeVector(0*cm, 0*cm, 0*cm), false));
      //G4cout << "mesh_IS530_Chamber_current" << G4endl;
      IS530_Chamber_current_sol.push_back( mesh_IS530_Chamber_current.at(i)->TessellatedMesh());
      //G4cout << "IS530_Chamber_current_sol" << G4endl;
      IS530_Chamber_current_log.push_back( new G4LogicalVolume(IS530_Chamber_current_sol.at(i), Mat.at(i), name+Form("IS530_Chamber_%i_log", i )));
      //G4cout << "IS530_Chamber_current_log" << G4endl;
      IS530_Chamber_current_log.at(i)	->SetVisAttributes(VisAttM.at(i));
      //G4cout << "SetVisAttributes" << G4endl;
      //IS530_Chamber_current_phys.push_back( new G4PVPlacement(0, G4ThreeVector(0, 0, 0), name+Form("/IS530_Chamber_%i_phys",i), IS530_Chamber_current_log.at(i), det_env, false, 0));
      IS530_Chamber_current_phys.push_back( new G4PVPlacement(transformation, name+Form("/IS530_Chamber_%i_phys",i), IS530_Chamber_current_log.at(i), mother, false, 0));
      //IS530_Chamber_current_phys.push_back( new G4PVPlacement(0, G4ThreeVector(0, 0, 0), name+Form("/IS530_Chamber_%i_phys",i), IS530_Chamber_current_log.at(i), mother, false, 0));
      //G4cout << "IS530_Chamber_current_phys" << G4endl;
    }

    return(mother);

}






















