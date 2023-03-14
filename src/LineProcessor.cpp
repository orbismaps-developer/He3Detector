//
// Created by Dan McCormick on 3/14/23.
//
#include "LineProcessor.h"
#include <G4NistManager.hh>
#include <G4tgbMaterialMgr.hh>
#include <G4UImessenger.hh>

G4bool LineProcessor::ProcessLine(const vector<G4String> &words)
{
    // process default text geometry tags
    G4bool processed = G4tgrLineProcessor::ProcessLine(words);
    if (processed) return true; // no new tags involved

    // process added tags: prop & surf
    G4String tag = words[0]; G4StrUtil::to_lower(tag); // to lower cases
    if (tag.substr(0,5)==":prop") { // set optical properties of a material
        G4NistManager *mgr = G4NistManager::Instance(); mgr->SetVerbose(2);
        G4Material *m = mgr->FindOrBuildMaterial(words[1]);
        if (m==NULL) // if not in NIST, then build in tgb
            m=G4tgbMaterialMgr::GetInstance()->FindOrBuildG4Material(words[1]);
        G4cout<<"GEARS: Set optical properties of "<<words[1]<<":"<<G4endl;
        m->SetMaterialPropertiesTable(CreateMaterialPropertiesTable(words,2));
        return true;
    } else if (tag.substr(0,5)==":surf") { // define an optical surface
        BorderSurface *bdr = new BorderSurface;
        bdr->next=Border; // save current border pointer
        Border=bdr; // overwrite current border pointer
        bdr->name=words[1];
        bdr->v1=words[2];
        bdr->v2=words[3];
        bdr->optic = new G4OpticalSurface(words[1]);
        size_t i=4;
        // loop over optical surface setup lines
        while (i<words.size()) {
            G4String setting = words[i], value = words[i+1];
            G4StrUtil::to_lower(setting); G4StrUtil::to_lower(value);
            if (setting=="property") {
                i++; break;
            } else if (setting=="type") {
                if (value=="dielectric_metal") bdr->optic->SetType(dielectric_metal);
                else if (value=="dielectric_dielectric")
                    bdr->optic->SetType(dielectric_dielectric);
                else if (value=="firsov") bdr->optic->SetType(firsov);
                else if (value=="x_ray") bdr->optic->SetType(x_ray);
                else G4cout<<"GERAS: Ignore unknown surface type "<<value<<G4endl;
            } else if (setting=="model") {
                if (value=="glisur") bdr->optic->SetModel(glisur);
                else if (value=="unified") bdr->optic->SetModel(unified);
                else G4cout<<"GERAS: Ignore unknown surface model "<<value<<G4endl;
            } else if (setting=="finish") {
                if (value=="polished") bdr->optic->SetFinish(polished);
                else if (value=="polishedfrontpainted")
                    bdr->optic->SetFinish(polishedfrontpainted);
                else if (value=="polishedbackpainted")
                    bdr->optic->SetFinish(polishedbackpainted);
                else if (value=="ground") bdr->optic->SetFinish(ground);
                else if (value=="groundfrontpainted")
                    bdr->optic->SetFinish(groundfrontpainted);
                else if (value=="groundbackpainted")
                    bdr->optic->SetFinish(groundbackpainted);
                else G4cout<<"GERAS: Ignore unknown surface finish "<<value<<G4endl;
            } else if (setting=="sigma_alpha") {
                bdr->optic->SetSigmaAlpha(G4UIcommand::ConvertToInt(value));
            } else
                G4cout<<"GERAS: Ignore unknown surface setting "<<value<<G4endl;
            i+=2;
        }
        if (i<words.size()) { // break while loop because of "property"
            G4cout<<"GEARS: Set optical properties of "<<bdr->name<<":"<<G4endl;
            bdr->optic->SetMaterialPropertiesTable(
                    CreateMaterialPropertiesTable(words,i));
        }
        return true;
    } else
        return false;
}
//______________________________________________________________________________
//
#include <G4tgrUtils.hh>
G4MaterialPropertiesTable* LineProcessor::CreateMaterialPropertiesTable(
        const vector<G4String> &words, size_t idxOfWords)
{
    bool photonEnergyUnDefined=true;
    int cnt=0; // number of photon energy values
    double *energies=NULL; // photon energy values
    G4MaterialPropertiesTable *table = new G4MaterialPropertiesTable();
    for (size_t i=idxOfWords; i<words.size(); i++) {
        G4String property = words[i]; G4StrUtil::to_upper(property);
        if (G4StrUtil::contains(property,"TIMECONSTANT") ||
            G4StrUtil::contains(property,"SCINTILLATIONYIELD") ||
            property=="RESOLUTIONSCALE" || property=="YIELDRATIO") {
            table->AddConstProperty(property, G4tgrUtils::GetDouble(words[i+1]));
            G4cout<<"GEARS: "<<property<<"="<<words[i+1]<<G4endl;
            i++; // property value has been used
        } else if (property.substr(0,12)=="PHOTON_ENERG") {
            photonEnergyUnDefined=false;
            cnt = G4UIcommand::ConvertToInt(words[i+1]); // get array size
            energies = new double[cnt]; // create energy array
            for (int j=0; j<cnt; j++)
                energies[j]=G4tgrUtils::GetDouble(words[i+2+j]);
            i=i+1+cnt; // array has been used
        } else { // wavelength-dependent properties
            if (photonEnergyUnDefined) {
                G4cout<<"GEARS: photon energies undefined, "
                      <<"ignore all wavelength-dependent properties!"<<G4endl;
                break;
            }
            double *values = new double[cnt];
            for (int j=0; j<cnt; j++) values[j]=G4tgrUtils::GetDouble(words[i+1+j]);
            G4cout<<"GEARS: "<<property<<"="<<values[0]<<", "
                  <<values[1]<<"..."<<G4endl;
            table->AddProperty(property, energies, values, cnt);
            delete[] values;
            i=i+cnt;
        }
    }
    delete[] energies;
    return table;
}
