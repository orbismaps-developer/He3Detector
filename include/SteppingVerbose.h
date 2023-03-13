
#ifndef HE3DETECTOR_STEPPINGVERBOSE_H
#define HE3DETECTOR_STEPPINGVERBOSE_H

#include <vector>
using namespace std;
#include <G4AnalysisManager.hh>
#include <G4SteppingVerbose.hh>
#include <G4SteppingManager.hh>
/**
 * Output simulation results to screen or a file.
 */
class SteppingVerbose : public G4SteppingVerbose
{
protected:
    void Record(); ///< Record simulated data
public:
    SteppingVerbose(); ///< Create analysis manager to handle output
    ~SteppingVerbose() {
        delete G4AnalysisManager::Instance();
    }

    void TrackingStarted() {
        G4SteppingVerbose::TrackingStarted();
        Record();
    } ///< Information of step 0 (initStep)

    void StepInfo() {
        G4SteppingVerbose::StepInfo();
        Record();
    } ///< Information of steps>0

    void Reset()  {
        trk.clear(); stp.clear(); vlm.clear(); pro.clear();
        pdg.clear(); pid.clear(); xx.clear(); yy.clear(); zz.clear(); dt.clear();
        de.clear(); dl.clear(); l.clear(); x.clear(); y.clear(); z.clear();
        t.clear(); k.clear(); p.clear(); q.clear(); et.clear();
    }

    void SetSteppingVerbose(int level) {
        fManager->SetVerboseLevel(level);
    }
    int GetSteppingVerbose() {
        return fManager->GetverboseLevel();
    }

    vector<int> trk;   ///< track ID
    vector<int> stp;   ///< step number
    vector<int> vlm;   ///< volume copy number
    vector<int> pro;   ///< process ID * 100 + sub-process ID
    vector<int> pdg;   ///< PDG encoding
    vector<int> pid;   ///< parent particle's PDG encoding
    vector<double> xx; ///< x [mm] (origin: center of local volume)
    vector<double> yy; ///< y [mm] (origin: center of local volume)
    vector<double> zz; ///< z [mm] (origin: center of local volume)
    vector<double> dt; ///< time elapsed from previous step point [ns]
    vector<double> de; ///< energy deposited [keV]
    vector<double> dl; ///< step length [mm]
    vector<double> l;  ///< length of track till this point [mm]
    vector<double> x;  ///< x [mm] (origin: center of the world)
    vector<double> y;  ///< y [mm] (origin: center of the world)
    vector<double> z;  ///< z [mm] (origin: center of the world)
    vector<double> t;  ///< time elapsed from the beginning of an event [ns]
    vector<double> k;  ///< kinetic energy [keV]
    vector<double> p;  ///< momentum [keV]
    vector<double> q;  ///< charge [elementary charge]
    vector<double> et; ///< Total energy deposited in a volume [keV]
};

#endif //HE3DETECTOR_STEPPINGVERBOSE_H
