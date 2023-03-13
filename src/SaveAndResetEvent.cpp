//
// Created by Dan McCormick on 3/13/23.
//

#include "SaveAndResetEvent.h"
#include <G4AnalysisManager.hh>
#include "SteppingVerbose.h"

void SaveAndResetEvent()
{
    auto a = G4AnalysisManager::Instance();
    SteppingVerbose* o = ((SteppingVerbose*) G4VSteppingVerbose::GetInstance());
    if (a->GetFileName()!="" && o->stp.size()!=0) {
        a->FillNtupleIColumn(0,o->stp.size());
        a->FillNtupleIColumn(1,o->et.size()-1);
        a->AddNtupleRow();
    } // save n-tuple if it is not empty and output file name is specified
    o->Reset(); // reset Output member variables for new record
} ///< save and then reset an event