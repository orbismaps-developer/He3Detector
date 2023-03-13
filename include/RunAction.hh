//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file RunAction.hh
/// \brief Definition of the RunAction class

#include <G4UserRunAction.hh>
#include <G4Run.hh>
#include <G4AnalysisManager.hh>
#include "SteppingVerbose.h"
/**
 * Book keeping before and after a run.
 */
class RunAction : public G4UserRunAction
{
public:
    void BeginOfRunAction (const G4Run*) {
        auto a = G4AnalysisManager::Instance(); if (a->GetFileName()=="") return;
        a->OpenFile();
        SteppingVerbose* o = ((SteppingVerbose*) G4VSteppingVerbose::GetInstance());
        if (o->GetSteppingVerbose()==0) { // in case of /tracking/verbose 0
            o->SetSilent(1); // avoid screen dump
            o->SetSteppingVerbose(1);//enable calling StepInfo() in G4SteppingManager
        }
    } ///< enable output if output file name is not empty
    void EndOfRunAction (const G4Run*) {
        auto a = G4AnalysisManager::Instance();
        if (a->GetFileName()!="") { a->Write(); a->CloseFile(); }
    }  ///< Close output file
};