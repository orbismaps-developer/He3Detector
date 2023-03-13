#include "He3GasMix.h"
#include "G4SystemOfUnits.hh"

He3GasMix::He3GasMix()
{
	G4NistManager* nistManager = G4NistManager::Instance();
	fMmolarConstant = CLHEP::Avogadro * CLHEP::k_Boltzmann;  //from clhep

	//G4Element* elC = nistManager->FindOrBuildElement("C");
	//G4Element* elO = nistManager->FindOrBuildElement("O");
	fElAr = nistManager->FindOrBuildElement("Ar");
	fCO2 = nistManager->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

	G4Isotope* He3Iso = new G4Isotope("He3", 2, 3);
	fElHe3 = new G4Element("Helium 3", "He3", 1);
	fElHe3->AddIsotope(He3Iso, 100 * perCent);

}

G4Material*  He3GasMix::Create(G4double pressure, G4double arFraction, G4double co2Fraction)
{
	G4double CO2Density = (fCO2->GetDensity() * pressure* co2Fraction) / (NTP_Temperature * fMmolarConstant);

	G4double ArMolarMass = 39.95 * g / mole;
	G4double ArDensity = (ArMolarMass * pressure * arFraction) / (NTP_Temperature * fMmolarConstant);
	G4double ArDensity___ = ArDensity * m3 / kg;

	G4double he3Fraction = 1 - arFraction - co2Fraction;
	G4double He3MolarMass = 3.016 * g / mole;
	G4double He3Density = (He3MolarMass * pressure * he3Fraction) / (NTP_Temperature * fMmolarConstant);
	G4double He3Density___ = He3Density * m3 / kg;

	G4double fillGasDensity = he3Fraction * He3Density + arFraction * ArDensity + co2Fraction * CO2Density;
	//G4Material::GetMaterialTable()->clear();
	G4Material* gasMix = new G4Material("He3GasMix", fillGasDensity, 3, kStateGas, NTP_Temperature, pressure);
	gasMix->AddElement(fElHe3, he3Fraction);
	gasMix->AddElement(fElAr, arFraction);
	gasMix->AddMaterial(fCO2, co2Fraction);
	return gasMix;
}

G4Material* He3GasMix::CreateDefault()
{
	return Create(10 * atmosphere, 0.2, 0.5);
}
