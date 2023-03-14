//
// Created by Dan McCormick on 3/14/23.
//

#include "detector.h"
#include <G4FieldManager.hh>
#include <G4UniformMagField.hh>
#include <G4TransportationManager.hh>
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include <G4tgrVolumeMgr.hh>
#include <G4tgrFileReader.hh>
#include <G4tgbVolumeMgr.hh>
#include <G4NistManager.hh>
#include <G4tgbMaterialMgr.hh>
#include "TextDetectorBuilder.h"

Detector::Detector(): G4VUserDetectorConstruction(), G4UImessenger(), fWorld(0)
{
#ifdef hasGDML
  fCmdOut = new G4UIcmdWithAString("/geometry/export",this);
  fCmdOut->SetGuidance("Export geometry gdml file name");
  fCmdOut->SetParameterName("gdml geometry output",false);
  fCmdOut->AvailableForStates(G4State_Idle);
#else
    fCmdOut=0;
#endif

    fCmdSrc = new G4UIcmdWithAString("/geometry/source",this);
    fCmdSrc->SetGuidance("Set geometry source file name");
    fCmdSrc->SetParameterName("text geometry input",false);
    fCmdSrc->AvailableForStates(G4State_PreInit);

    fCmdSetB = new G4UIcmdWith3VectorAndUnit("/geometry/SetB",this);
    fCmdSetB->SetGuidance("Set uniform magnetic field value.");
    fCmdSetB->SetParameterName("Bx", "By", "Bz", false);
    fCmdSetB->SetUnitCategory("Magnetic flux density");
}
//______________________________________________________________________________
//

#ifdef hasGDML
#include "G4GDMLParser.hh"
#endif
void Detector::SetNewValue(G4UIcommand* cmd, G4String value)
{
    if (cmd==fCmdSetB) {
        G4UniformMagField* field = new G4UniformMagField(0,0,0);
        field->SetFieldValue(fCmdSetB->GetNew3VectorValue(value));
        G4FieldManager* mgr =
                G4TransportationManager::GetTransportationManager()->GetFieldManager();
        mgr->SetDetectorField(field);
        mgr->CreateChordFinder(field);
        G4cout<<"GEARS: Magnetic field is set to "<<value<<G4endl;
#ifdef hasGDML
        } else if(cmd==fCmdOut) {
    G4GDMLParser paser;
    paser.Write(value,fWorld);
#endif
    } else { // cmd==fCmdSrc
        if (value.substr(value.length()-4)!="gdml") { // text geometry input
            G4tgbVolumeMgr* mgr = G4tgbVolumeMgr::GetInstance();
            mgr->AddTextFile(value);
            TextDetectorBuilder * tgb = new TextDetectorBuilder;
            mgr->SetDetectorBuilder(tgb);
            fWorld = mgr->ReadAndConstructDetector();
#ifdef hasGDML
            } else { // GDML input
      G4GDMLParser parser;
      parser.Read(value);
      fWorld=parser.GetWorldVolume();
#endif
        }
    }
}
//______________________________________________________________________________
//

G4VPhysicalVolume* Detector::Construct()
{
    if (fWorld==NULL) {
        G4cout<<"GEARS: no detector specified, set to a 10x10x10 m^3 box."<<G4endl;
        G4Box* box = new G4Box("hall", 5*CLHEP::m, 5*CLHEP::m, 5*CLHEP::m);
        G4NistManager *nist = G4NistManager::Instance();
        G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        G4LogicalVolume *v = new G4LogicalVolume(box, vacuum, "hall");
        fWorld = new G4PVPlacement(0, G4ThreeVector(), v, "hall", 0, 0, 0);
    }
    return fWorld;
}
