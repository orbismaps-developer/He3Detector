//
// Created by Dan McCormick on 3/14/23.
//

#ifndef HE3DETECTOR_LINEPROCESSOR_H
#define HE3DETECTOR_LINEPROCESSOR_H

#include <G4tgrLineProcessor.hh>
#include <G4NistManager.hh>
#include <G4tgbMaterialMgr.hh>
#include <G4OpticalSurface.hh>
#include <vector>
using namespace std;

struct BorderSurface
{
    G4String name; ///< name of the surface
    G4String v1;   ///< name of volume 1
    G4String v2;   ///< name of volume 2
    G4OpticalSurface* optic; ///< point to G4OpticalSurface object
    BorderSurface* next; ///< link to next border surface
};


class LineProcessor: public G4tgrLineProcessor
{
private:
    G4MaterialPropertiesTable* CreateMaterialPropertiesTable(
            const vector<G4String> &words, size_t idxOfWords);
public:
    LineProcessor(): G4tgrLineProcessor(), Border(0) {};
    ~LineProcessor() {
        while (Border) { // deleting G4OpticalSurface is done in Geant4
            BorderSurface *next=Border->next;
            delete Border;
            Border=next;
        }
    }
    /**
     * Overwrite G4tgrLineProcessor::ProcessLine to add new tags.
     *
     * Two new tags are added: ":PROP" and ":SURF" (case insensitive):
     * - ":prop" is used to add optical properties to a material
     * - ":surf" is used to define an optical surface
     *
     * The function is called for each new line.  Be sure to insert an
     * end-of-line character by typing <Enter> at the end of the last line,
     * otherwise, the last line will not be processed.
     */
    G4bool ProcessLine(const vector<G4String> &words);
    BorderSurface* Border; ///< pointer to a BorderSurface object
};
#endif //HE3DETECTOR_LINEPROCESSOR_H
