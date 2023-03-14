//
// Created by Dan McCormick on 3/14/23.
//

#ifndef HE3DETECTOR_DETECTOR_H
#define HE3DETECTOR_DETECTOR_H

#include <G4UImessenger.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4VUserDetectorConstruction.hh>


class Detector : public G4VUserDetectorConstruction, public G4UImessenger
{
public:
    Detector();
    ~Detector() { delete fCmdSetB; delete fCmdSrc; delete fCmdOut; }
    G4VPhysicalVolume* Construct(); ///< called at /run/initialize
    void SetNewValue(G4UIcommand* cmd, G4String value); ///< for G4UI

private:
    G4UIcmdWith3VectorAndUnit* fCmdSetB; ///< /field/setB
    G4UIcmdWithAString* fCmdSrc; ///< /geometry/source
    G4UIcmdWithAString* fCmdOut; ///< /geometry/export
    G4VPhysicalVolume * fWorld;
};

#endif //HE3DETECTOR_DETECTOR_H
