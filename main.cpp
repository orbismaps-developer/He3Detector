#include <iostream>
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include <G4ScoringManager.hh>
#include "SteppingVerbose.h"
#include "RunManager.h"
#include "ExTGDetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"
/**
 * The main function that calls individual components.
 */
int main(int argc, char **argv)
{
    // inherit G4SteppingVerbose instead of G4UserSteppingAction to record data
    G4VSteppingVerbose::SetInstance(new SteppingVerbose); // must be before run manager
    RunManager* run = new RunManager; // customized run control
    run->SetUserInitialization(new ExTGDetectorConstruction);

    auto physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    run->SetUserInitialization(physicsList);

    G4ScoringManager::GetScoringManager(); // enable built-in scoring cmds
    G4VisManager* vis = new G4VisExecutive("quiet"); // visualization
    vis->Initialize();
    // select mode of execution
    if (argc!=1)  { // batch mode
        G4String cmd = "/control/execute ";
        G4UImanager::GetUIpointer()->ApplyCommand(cmd+argv[1]);
    } else { // interactive mode
        // check available UI automatically in the order of Qt, tsch, Xm
        G4UIExecutive *ui = new G4UIExecutive(argc,argv);
        ui->SessionStart();
        delete ui;
    }
    // clean up
    delete vis; delete run;
    return 0;
}
