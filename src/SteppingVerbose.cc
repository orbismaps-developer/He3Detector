#include <vector>
using namespace std;
#include <G4AnalysisManager.hh>
#include <G4SteppingVerbose.hh>
#include <G4SteppingManager.hh>
#include "SteppingVerbose.h"
/**
 * Output simulation results to screen or a file.
 */

//______________________________________________________________________________
//
SteppingVerbose::SteppingVerbose(): G4SteppingVerbose()
{
    auto manager = G4AnalysisManager::Instance();
    manager->CreateNtuple("t", "Geant4 step points");
    manager->CreateNtupleIColumn("n"); // total number of recorded hits
    manager->CreateNtupleIColumn("m"); // max copy number of sensitive volume
    manager->CreateNtupleIColumn("trk", trk);
    manager->CreateNtupleIColumn("stp", stp);
    manager->CreateNtupleIColumn("vlm", vlm);
    manager->CreateNtupleIColumn("pro", pro);
    manager->CreateNtupleIColumn("pdg", pdg);
    manager->CreateNtupleIColumn("pid", pid);
    manager->CreateNtupleDColumn("xx", xx);
    manager->CreateNtupleDColumn("yy", yy);
    manager->CreateNtupleDColumn("zz", zz);
    manager->CreateNtupleDColumn("dt", dt);
    manager->CreateNtupleDColumn("de", de);
    manager->CreateNtupleDColumn("dl", dl);
    manager->CreateNtupleDColumn("l", l);
    manager->CreateNtupleDColumn("x", x);
    manager->CreateNtupleDColumn("y", y);
    manager->CreateNtupleDColumn("z", z);
    manager->CreateNtupleDColumn("t", t);
    manager->CreateNtupleDColumn("k", k);
    manager->CreateNtupleDColumn("p", p);
    manager->CreateNtupleDColumn("q", q);
    manager->CreateNtupleDColumn("et", et);
    manager->FinishNtuple();
}
//______________________________________________________________________________
//
#include <G4NavigationHistory.hh>
void SteppingVerbose::Record()
{
    if (GetSilent()==1) // CopyState() won't be called in G4SteppingVerbose
        CopyState(); // point fTrack, fStep, etc. to right places

    G4TouchableHandle handle = fStep->GetPreStepPoint()->GetTouchableHandle();
    int copyNo=handle->GetReplicaNumber();
    if (copyNo<=0) return; //skip uninteresting volumes (copy No. of world == 0)
    if (trk.size()>=10000) {
        G4cout<<"GEARS: # of step points >=10000. Recording stopped."<<G4endl;
        fTrack->SetTrackStatus(fKillTrackAndSecondaries);
        return;
    }

    trk.push_back(fTrack->GetTrackID());
    stp.push_back(fTrack->GetCurrentStepNumber());
    vlm.push_back(copyNo);
    pdg.push_back(fTrack->GetDefinition()->GetPDGEncoding());
    pid.push_back(fTrack->GetParentID());
    if (stp.back()==0) { // step zero
        if (pid.back()!=0) // not primary particle
            pro.push_back(fTrack->GetCreatorProcess()->GetProcessType()*1000
                          + fTrack->GetCreatorProcess()->GetProcessSubType());
        else pro.push_back(1000); // primary particle
    } else {
        const G4VProcess *pr = fStep->GetPostStepPoint()->GetProcessDefinedStep();
        if (pr) pro.push_back(pr->GetProcessType()*1000 + pr->GetProcessSubType());
        else pro.push_back(900); // not sure why pr can be zero
    }

    k.push_back(fTrack->GetKineticEnergy()/CLHEP::keV);
    p.push_back(fTrack->GetMomentum().mag()/CLHEP::keV);
    q.push_back(fStep->GetPostStepPoint()->GetCharge());
    l.push_back(fTrack->GetTrackLength()/CLHEP::mm);

    de.push_back(fStep->GetTotalEnergyDeposit()/CLHEP::keV);
    dl.push_back(fTrack->GetStepLength()/CLHEP::mm);

    t.push_back(fTrack->GetGlobalTime()/CLHEP::ns);
    x.push_back(fTrack->GetPosition().x()/CLHEP::mm);
    y.push_back(fTrack->GetPosition().y()/CLHEP::mm);
    z.push_back(fTrack->GetPosition().z()/CLHEP::mm);

    G4ThreeVector pos = handle->GetHistory()->GetTopTransform()
            .TransformPoint(fStep->GetPostStepPoint()->GetPosition());
    xx.push_back(pos.x()/CLHEP::mm);
    yy.push_back(pos.y()/CLHEP::mm);
    zz.push_back(pos.z()/CLHEP::mm);
    dt.push_back(fTrack->GetLocalTime()/CLHEP::ns);

    if (de.back()>0 && G4StrUtil::contains(handle->GetVolume()->GetName(),"(S)")) {
        if (et.size()<(unsigned int)copyNo+1) et.resize((unsigned int)copyNo+1);
        et[copyNo]+=de.back(); et[0]+=de.back();
    }
}
