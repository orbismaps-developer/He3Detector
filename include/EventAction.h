//
// Created by Dan McCormick on 3/14/23.
//

#ifndef HE3DETECTOR_EVENTACTION_H
#define HE3DETECTOR_EVENTACTION_H

#include <G4UserEventAction.hh>
#include "SaveAndResetEvent.h"

/**
 * Book keeping before and after an event.
 */
class EventAction : public G4UserEventAction {
    public: void EndOfEventAction(const G4Event*) {
        SaveAndResetEvent();
    }
};

#endif //HE3DETECTOR_EVENTACTION_H
