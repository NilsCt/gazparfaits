#include "Transformation.h"
#include "Enceinte.h"
#include "Vecteur3D.h"
#include <iostream>
#include <cmath>

using namespace std;

Transformation::Transformation(double duree_) : duree(duree_), debut(0) {
    if(duree < 1e-8) throw "Durée invalide (nulle)";
}

void Transformation::demarrer(double debut_, double t_i __attribute__((unused)), double v_i __attribute__((unused)) , double time_ratio __attribute__((unused))) {
    debut = debut_; 
}

bool Transformation::est_terminee(double t, double ratio) const {
    return t >= debut + duree * ratio;
}


ModificationTemperature::ModificationTemperature(double t_f, double duree_) :
    Transformation(duree_), temperature_finale(t_f), coef_chauffe(0) {}
    // on initialise le coef de chauffe au démarrage


void ModificationTemperature::demarrer(double debut_, double t_i, double v_i, double time_ratio) {
    Transformation::demarrer(debut_, t_i, v_i, time_ratio);
    coef_chauffe = log(temperature_finale / t_i) / (duree*time_ratio);
}

double ModificationTemperature::get_coef_chauffe(double t __attribute__((unused)), double dt, double gamma __attribute__((unused))) const {
    return exp(coef_chauffe*dt/2); // diviser par 2 car v2 proportionnel à T
}


ModificationEnceinte::ModificationEnceinte(double l_f, double p_f, double h_f, double dur) :
    Transformation(dur), largeur_finale(l_f), profondeur_finale(p_f), hauteur_finale(h_f) {}

Enceinte* ModificationEnceinte::construire_enceinte(double l_i, double p_i, double h_i) const {
    return new EnceinteDeformable(
            h_i,
            l_i,
            p_i,
            hauteur_finale,
            largeur_finale,
            profondeur_finale,
            duree
            );
}


TravailAdiabatique::TravailAdiabatique(double l_f, double p_f, double h_f, double dur) :
    ModificationEnceinte(l_f, p_f, h_f, dur), volume_initial(0), coef_compression(0) {}

void TravailAdiabatique::demarrer(double debut_, double t_i, double v_i, double time_ratio) {
    Transformation::demarrer(debut_, t_i, v_i, time_ratio);
    volume_initial = v_i;
    coef_compression = ((largeur_finale * profondeur_finale * hauteur_finale)*pow(Vecteur3D::unite_espace, 3) - volume_initial) / (duree*time_ratio);
}

double TravailAdiabatique::get_coef_chauffe(double t, double dt, double gamma) const {
    return exp((1-gamma) * log(1 + coef_compression * dt / (volume_initial + coef_compression * (t-debut))) /2); // disivisé par 2 car v2 est proportionnel à T

    //return -3/3 / (volume_initial/coef_compression + t); // néglige t et approxime ln
}
