//
// Created by Dan McCormick on 3/14/23.
//

#ifndef HE3DETECTOR_TEXTDETECTORBUILDER_H
#define HE3DETECTOR_TEXTDETECTORBUILDER_H

#include <G4tgbDetectorBuilder.hh>
#include "LineProcessor.h"

/**
 * Construct detector based on text geometry description.
 */
class TextDetectorBuilder : public G4tgbDetectorBuilder
{
public :
    TextDetectorBuilder() :
            G4tgbDetectorBuilder() { fLineProcessor = new LineProcessor(); }
    ~TextDetectorBuilder() { delete fLineProcessor; }
    const G4tgrVolume* ReadDetector(); ///< Read text geometry input
    /**
     * Construct detector based on text geometry description.
     */
    G4VPhysicalVolume* ConstructDetector(const G4tgrVolume* topVol);

private :
    LineProcessor* fLineProcessor; ///< Process individual lines in a tg file
};

#endif //HE3DETECTOR_TEXTDETECTORBUILDER_H
