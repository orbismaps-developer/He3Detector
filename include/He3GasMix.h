#pragma once
#include "G4Material.hh"
#include "G4NistManager.hh"

class He3GasMix
{
public:
	He3GasMix();
	G4Material* Create(G4double pressure, G4double arFraction, G4double co2Fraction);
	G4Material* CreateDefault();

private:
	G4Element* fElHe3;
	G4Element* fElAr;
	G4Material* fCO2;
	G4double fMmolarConstant;
};

