#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4GeneralParticleSource.hh>
/**
 * Call Geant4 General Particle Source to generate particles.
 */
class Generator : public G4VUserPrimaryGeneratorAction
{
private:
    G4GeneralParticleSource* fSource;
public:
    Generator() : G4VUserPrimaryGeneratorAction(), fSource(0) {
        fSource = new G4GeneralParticleSource;
    }

    virtual ~Generator() {
        delete fSource;
    }
    virtual void GeneratePrimaries(G4Event* evt) {
        fSource->GeneratePrimaryVertex(evt);
    } ///< add sources to an event
};
