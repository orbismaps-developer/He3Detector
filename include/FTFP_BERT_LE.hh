#pragma once

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class FTFP_BERT_LE : public G4VModularPhysicsList
{
public:
	FTFP_BERT_LE(G4int ver = 1);
	virtual ~FTFP_BERT_LE() = default;

	FTFP_BERT_LE(const FTFP_BERT_LE&) = delete;
	FTFP_BERT_LE& operator=(const FTFP_BERT_LE&) = delete;

};
