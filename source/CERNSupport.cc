
// $Id: CERNSupport.cc 07.05.2017 A Fijalkowska $
//
/// \file CERNSupport.cc
/// \brief Implementation of the ArgoneVandleArray class
//
//

#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "CERNSupport.hh"
#include "CADMesh.hh"

CERNSupport::CERNSupport()
{
    materialsManager = MaterialsManager::GetInstance();
    
    argSupportVisAtt = new G4VisAttributes( G4Colour(248./255.,248./255.,255./255.)); 
	argSupportVisAtt->SetForceAuxEdgeVisible(true);// Can see outline when drawn with lines
	argSupportVisAtt->SetForceSolid(true);
	argoneFrame = new ArgoneFrame();
	SetBasicSizes();
} 

CERNSupport::~CERNSupport()
{

}

void CERNSupport::SetBasicSizes()
{
    ribX = argoneFrame->GetRibsX();
    ribY = argoneFrame->GetRibsY();
    ribZ = argoneFrame->GetRibsZ();	
}


void CERNSupport::Place(G4RotationMatrix *pRot, 
                        G4ThreeVector &tlate, 
                        const G4String &pName, 
                        G4LogicalVolume *pMotherLogical,  
                        G4int pCopyNo)
{

    frameMaterial = materialsManager->GetAluminum();
    
	G4VSolid* ribsSolid =  argoneFrame->GetRibsSolid();
    //G4VSolid* ribsTotSolid = AddHolders(ribsSolid); 

    G4LogicalVolume* ribLogical = new G4LogicalVolume(ribsSolid, 
                                                      frameMaterial, 
                                                      "ribsTotSolid");
                                                      
                                                      
                                                      
    /*G4LogicalVolume* ribLogical = new G4LogicalVolume(ribsTotSolid, 
                                                      frameMaterial, 
                                                      "ribsTotSolid"); */
	ribLogical->SetVisAttributes(argSupportVisAtt); 
	//left
	G4double distRibs = 45.593/2.0*cm;
	const G4ThreeVector posBoxBL(0.0,ribZ+distRibs,ribY);
	if(pRot == 0L)
	    pRot = new G4RotationMatrix();
	pRot->rotateX(-90.0*deg);
	G4ThreeVector posLeft = tlate + posBoxBL;
	new G4PVPlacement( pRot, 
	                   posLeft, 
	                   ribLogical, 
	                   "boxBLPhysical", 
	                   pMotherLogical,
	                   0, 
	                   0 ); 
	//right
	G4RotationMatrix rotSec(*pRot);
	rotSec.rotateY(180.0*deg);
	const G4ThreeVector posBoxBR(0.0,-(ribZ+distRibs),ribY);
	G4ThreeVector posRight = tlate + posBoxBR;

	G4Transform3D transformBoxBR(rotSec,posRight);
	new G4PVPlacement( transformBoxBR, 
	                   ribLogical, 
	                   "boxBRPhysical", 
	                   pMotherLogical, 
	                   0, 
	                   0 ); 
   //G4ThreeVector offset = G4ThreeVector(0, 0, 0);
   std::string file1 = "/ARCHIVE/Ddata/geant4_stl/vandle/isolde/VerticalFrame.stl";
   CADMesh * rebMesh = new CADMesh((char*)file1.c_str());
   rebMesh->SetScale(cm);

   //CADMesh* rebMesh = new CADMesh("/ARCHIVE/Ddata/geant4_stl/vandle/isolde/VerticalFrame.stl", cm, offset, false);
   G4VSolid* ribSolid =  rebMesh->TessellatedMesh();
   G4LogicalVolume* ribLogic = new G4LogicalVolume(ribSolid, frameMaterial, "CERNRrameRib");
   ribLogic->SetVisAttributes(argSupportVisAtt);
   
   G4RotationMatrix ribRot(*pRot);
   ribRot.rotateY(-90.0*deg);
   G4ThreeVector ribPos(-1.225*m,0, 163.*cm);
   ribPos = tlate + ribPos;
   G4Transform3D transfRib(ribRot,ribPos);     
   new G4PVPlacement(transfRib,ribLogic,"frameRibs", pMotherLogical, 0, 0);
	
	
	//PlaceSkeleton(pRot, tlate, pName, pMotherLogical, pCopyNo);
//112.9284

}


G4VSolid* CERNSupport::AddHolders(G4VSolid*ribsSolid)
{

	
}

void CERNSupport::PlaceSkeleton(G4RotationMatrix *pRot, 
                                G4ThreeVector &tlate, 
                                const G4String &pName, 
                                G4LogicalVolume *pMotherLogical,  
                                G4int pCopyNo)
{
						
							
}
