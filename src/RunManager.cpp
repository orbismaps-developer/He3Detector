#include "RunManager.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.h"

void RunManager::InitializePhysics() {
    G4RunManager::InitializePhysics(); // call the original function
    // has to be called after physics initialization
    SetUserAction(new Generator);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    SetUserAction(new StackingAction);
    } ///< set physics list if it is not specified explicitly
