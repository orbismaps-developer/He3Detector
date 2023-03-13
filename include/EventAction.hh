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
