
#ifndef HE3DETECTOR_RUNMANAGER_H
#define HE3DETECTOR_RUNMANAGER_H

#include <G4RunManager.hh>
#include <G4StateManager.hh>
#include <G4PhysListFactory.hh>
#include <G4UIcmdWithAString.hh>

/**
 * Place to put all building blocks together.
 */
class RunManager : public G4RunManager, public G4UImessenger {

public:
    void InitializePhysics();
    RunManager() : G4RunManager(), G4UImessenger() { }
};
#endif //HE3DETECTOR_RUNMANAGER_H
