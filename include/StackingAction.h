//
// Created by Dan McCormick on 3/12/23.
//

#ifndef HE3DETECTOR_STACKINGACTION_H
#define HE3DETECTOR_STACKINGACTION_H

#include <G4UserStackingAction.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UImessenger.hh>
#include "SteppingVerbose.h"

class StackingAction : public G4UserStackingAction, public G4UImessenger
{
private:
    double fT0; ///< reference time for splitting decay chain
    double fTimeWindow; ///< time window to split a decay chain
    G4UIcmdWithADoubleAndUnit *fCmdT; ///< UI cmd to set time window
public:
    StackingAction() : G4UserStackingAction(), G4UImessenger(),
                       fT0(0), fTimeWindow(0), fCmdT(0) {
        fCmdT = new G4UIcmdWithADoubleAndUnit("/grdm/setTimeWindow", this);
        fCmdT->SetGuidance("Time window to split a radioactive decay chain.");
        fCmdT->SetGuidance("If a daughter nucleus appears after the window,");
        fCmdT->SetGuidance("it is saved in a new entry in the output ntuple.");
        fCmdT->SetGuidance("---Set it to <=0 to disable the splitting---");
        fCmdT->SetParameterName("time window",false,true);
        fCmdT->SetDefaultUnit("s");
        fCmdT->AvailableForStates(G4State_PreInit, G4State_Idle);
    } ///< created macro /grdm/setTimeWindow
    ~StackingAction() { delete fCmdT; }
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *trk) {
        if (fTimeWindow<=0) return fUrgent; // no need to split
        if (trk->GetGlobalTime()>fT0+fTimeWindow) return fWaiting; // split
        else return fUrgent; // too fast to be split
    } ///< send a daughter particle to waiting stack if it appears too late
    void NewStage() {
        // called after processing urgent trk, before waiting trk
        if (fTimeWindow<=0) return; // do nothing if no time window is specified
        SteppingVerbose* o = ((SteppingVerbose*) G4VSteppingVerbose::GetInstance());
        fT0 = o->t.back(); // update the reference time to the latest decay time
    } ///< save and reset output before processing waiting tracks

    void SetNewValue(G4UIcommand* cmd, G4String value)
    {
        if (cmd!=fCmdT) return; fTimeWindow = fCmdT->GetNewDoubleValue(value);
    }
};

#endif //HE3DETECTOR_STACKINGACTION_H
