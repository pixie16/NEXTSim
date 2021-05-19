/************************************************************************
 * \file Polyhedron.hh
 *
 * \class Polyhedron   
 * \brief Design of the Polyhedron frame holding the HPGe detectors
 *
 * \author Christophe Sotty : csotty@gmail.com
 *                            Christophe.Sotty@fys.kuleuven.be
 *
 *************************************************************************/

#include "Polyhedron.hh"
#include "CADMesh.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include <TText.h>

using namespace std;



Polyhedron::Polyhedron(G4VPhysicalVolume* p_mother, G4double p_rho, G4double p_theta, G4double p_phi, G4double p_spin)
  :mother(p_mother), rho(p_rho), theta(p_theta), phi(p_phi), spin(p_spin)
{
	//G4cout	<<"\nPolyhedron Frame (OSIRIS) #######################\n"	<<flush
	//	<<"\nRho =\t\t"		<<rho/mm	<< " mm"	<<flush
	//	<<"\nTheta =\t\t"	<<theta/deg	<< " deg" 	<<flush
	//	<<"\nPhi =\t\t"		<<phi/deg	<< " deg"       <<flush
	//	<<"\nSpin =\t\t"	<<spin/deg	<< " deg"       <<flush<<G4endl;
}

Polyhedron::~Polyhedron()
{;}


G4VPhysicalVolume* Polyhedron::Construct()
{
    // Naming
    //
    G4String name="/Polyhedron";
    
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
    Polyhedron_vis_att = new G4VisAttributes(G4Colour(0.04,0.85,0.32)); 
    Polyhedron_vis_att->SetForceSolid(true);  
    G4VisAttributes* Al_vis_att 	= new G4VisAttributes(G4Colour(0.65,0.65,0.65));
    Al_vis_att->SetForceSolid(true);

    phi = phi + 90.*deg;
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
    //det_env_log -> SetVisAttributes(G4VisAttributes::Invisible);
    //det_env		= new G4PVPlacement(*Polyhedron_transformation, name+"/env", det_env_log, mother, false, 0);
    

    // Detector Construction
    //org 64 66 70 72 76 79 82 83
    vector<char* > Polyhedron_name;
    
    /*  */
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_64.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_65.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_66.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_70.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_71.stl");
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_72.stl");
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_74.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_75.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_76.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_79.stl");
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_80.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_81.stl");
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_82.stl");
    
    
    
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_83.stl");
    
    
    //**************************************
    
    
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_85.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_89.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_91.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_hexagon_92.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_66.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_67.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_69.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_70.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_73.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_74.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_76.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_77.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_78.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_79.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_83.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_87.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_88.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_octogon_97.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_63.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_64.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_67.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_68.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_69.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_72.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_73.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_74.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_75.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_76.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_77.stl");


//****************************************************************

    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_78.stl");   
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_79.stl"); 
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_80.stl");   
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_81.stl");
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_82.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_83.stl");   
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_84.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_85.stl");
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_86.stl");    
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_87.stl");
    
 
 
 
 
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_88.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_90.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_93.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_94.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_95.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_patrat_96.stl");
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_100.stl");
   // Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_101.stl");


//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_102.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_103.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_104.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_105.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_88.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_89.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_90.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_91.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_92.stl");


    //front cap
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_93.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_94.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_95.stl");
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_96.stl");
    
    
    
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_98.stl"); 
    
    //front cap   
    //Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_capac_circular_99.stl");
    
    
    
    
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_103.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_104.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_107.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_109.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_110.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_113.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_119.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_131.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_132.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_141.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_97.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_prindere_detector_patrat_98.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_102.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_103.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_106.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_108.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_109.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_112.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_118.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_130.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_131.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_140.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_96.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_intermediara_detector_patrat_97.stl");



    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_114.stl");
    
   
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_115.stl");

    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_118.stl");
    
    
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_119.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_122.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_123.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_124.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_126.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_127.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_128.stl");



    //pices around Ge
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_132.stl");  
    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_flansa_fixa_pe_prisma_2_136.stl");
    
    
    
    
    
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_talpa1_28.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_talpa_23.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_talpa_53.stl");



    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_Placa_suport_polietru1_4.stl");
    
    
    
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_disc_5.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_disc_7.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/IDS_5_disc_9.stl");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");
//    Polyhedron_name.push_back("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/IDS_5/");

    // Meshing and Logical Volumes
    for(int i = 0; i<Polyhedron_name.size(); i++){ 
    //for(int i = 0; i<1; i++){ 
      //G4cout<< Polyhedron_name.at(i) << G4endl;
      //mesh_Polyhedron_current.push_back( new CADMesh(const_cast<char*>(Polyhedron_name.at(i)), mm, G4ThreeVector(109.41*cm, 42.58*cm, -50.305*cm), false));
      mesh_Polyhedron_current.push_back( new CADMesh(Polyhedron_name.at(i), mm, G4ThreeVector(109.41*cm, 42.58*cm, -50.305*cm), false));
      //G4cout << "mesh_Polyhedron_current" << G4endl;
      Polyhedron_current_sol.push_back( mesh_Polyhedron_current.at(i)->TessellatedMesh());
      //G4cout << "Polyhedron_current_sol" << G4endl;
      Polyhedron_current_log.push_back( new G4LogicalVolume(Polyhedron_current_sol.at(i), Al_mat, name+Form("Poly_%i_log", i )));
      //G4cout << "Polyhedron_current_log" << G4endl;
      Polyhedron_current_log.at(i)	->SetVisAttributes(Polyhedron_vis_att);
      if(i==(Polyhedron_name.size()-1))	Polyhedron_current_log.at(i)->SetVisAttributes(Al_vis_att);
      //G4cout << "SetVisAttributes" << G4endl;
      //Polyhedron_current_phys.push_back( new G4PVPlacement(0, G4ThreeVector(0, 0, 0), name+Form("/Poly_%i_phys",i), Polyhedron_current_log.at(i), det_env, false, 0));
      Polyhedron_current_phys.push_back( new G4PVPlacement(transformation, name+Form("/Poly_%i_phys",i), Polyhedron_current_log.at(i), mother, false, 0));
      //Polyhedron_current_phys.push_back( new G4PVPlacement(0, G4ThreeVector(0, 0, 0), name+Form("/Poly_%i_phys",i), Polyhedron_current_log.at(i), mother, false, 0));
      //G4cout << "Polyhedron_current_phys" << G4endl;
    }

    return(mother);

}




