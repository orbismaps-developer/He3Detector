#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIcmdWithADoubleAndUnit;
class PrimaryGeneratorAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

class  PrimaryGeneratorMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    virtual ~PrimaryGeneratorMessenger();
    virtual void SetNewValue(G4UIcommand*, G4String);

private:
    PrimaryGeneratorAction* fGenerator;
    G4UIdirectory* fGeneratorDirectory;
    G4UIcmdWithADoubleAndUnit* fNeutronEnergy;
    G4UIcmdWithAnInteger* fNeutronsCapturedTargetCmd;
};


#endif