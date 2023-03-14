
#ifndef HE3DETECTOR_RUNMANAGER_H
#define HE3DETECTOR_RUNMANAGER_H

#include <G4RunManager.hh>
#include <G4StateManager.hh>
#include <G4PhysListFactory.hh>
#include "detector.h"
#include "Generator.h"
#include "RunAction.h"
#include "EventAction.h"
#include "StackingAction.h"

/**
 * Place to put all building blocks together.
 */
class RunManager : public G4RunManager, public G4UImessenger
{
private:
    G4PhysListFactory* fFactory; ///< tool to construct a ref. list by its name
    G4UIcmdWithAString* fCmdPhys; ///< macro cmd to select a physics list
public:
    RunManager() : G4RunManager(), G4UImessenger(), fFactory(0) {
        SetUserInitialization(new Detector); // needed for /run/initialize
        fCmdPhys = new G4UIcmdWithAString("/physics_lists/select",this);
        fCmdPhys->SetGuidance("Select a physics list");
        fCmdPhys->SetGuidance("Candidates are specified in G4PhysListFactory.cc");
        fCmdPhys->SetParameterName("name of a physics list", false);
        fCmdPhys->AvailableForStates(G4State_PreInit);
    }
    ~RunManager() { delete fCmdPhys; delete fFactory; }

    void SetNewValue(G4UIcommand* cmd, G4String value) {
        if (cmd!=fCmdPhys || fFactory) return;
        fFactory = new G4PhysListFactory;
        if (fFactory->IsReferencePhysList(value)==false) {
            G4cout<<"GEARS: no physics list \""<<value
                  <<"\", set to \"QGSP_BERT\""<<G4endl;
            value = "QGSP_BERT"; // default
        }
        SetUserInitialization(fFactory->GetReferencePhysList(value));
    } ///< for UI

    void InitializePhysics() {
        if (fFactory==NULL) { // no /physics_lists/select is used
            fFactory = new G4PhysListFactory;
            G4StateManager::GetStateManager()->SetNewState(G4State_PreInit);
            // has to be called in PreInit state:
            SetUserInitialization(fFactory->GetReferencePhysList("QGSP_BERT"));
        }
        G4RunManager::InitializePhysics(); // call the original function
        // has to be called after physics initialization
        SetUserAction(new Generator);
        SetUserAction(new RunAction);
        SetUserAction(new EventAction);
        SetUserAction(new StackingAction);
    } ///< set physics list if it is not specified explicitly
};

#endif //HE3DETECTOR_RUNMANAGER_H
