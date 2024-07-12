#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <cmath>
#include <memory>
#include "Particule.h"
#include "Enceinte.h"
#include "GenerateurAleatoire.h"
#include "SupportADessin.h"
#include "Systeme.h"

using namespace std;

//  Constructeurs
Particule::Particule(Vecteur3D const& position, Vecteur3D const& vitesse, double masse, bool est_suivi) :
position(position), vitesse(vitesse), masse(masse), suivi(nullptr) {
    if(est_suivi) {
       suivi = make_unique<Suivi>(*this);
    }
}

Particule::Particule(const Particule& particule) :
position(particule.position), vitesse(particule.vitesse), masse(particule.masse) {
    if(suivi != nullptr) {
     	suivi = make_unique<Suivi>(*this);
    }
}

Particule& Particule::operator=(const Particule& particule) {
    position = particule.position;
    vitesse = particule.vitesse;
    // la masse est constante
    suivi = make_unique<Suivi>(*this); // propre à chaque instance
    return *this;
}

Particule::Particule(Particule&& particule) : position(std::move(particule.position)), vitesse(std::move(particule.vitesse)), masse(std::move(particule.masse)), suivi(std::move(particule.suivi)) {}

Particule& Particule::operator=(Particule&& particule) {
    position = std::move(particule.position);
    vitesse = std::move(particule.vitesse);
    // la masse est constante
    std::swap(suivi, particule.suivi);
    return *this;
}


//  Méthodes
ostream& Particule::affiche(ostream& sortie) const {
    // le setprecision permet de correspondre aux affichages de l'énoncé
    marqueur_suivi(sortie); // repère les particules dont on suit l'avancement
    return sortie << "pos : " << position << " ; v : " << vitesse << " ; m : " << setprecision(8) << masse << setprecision(6) << endl;
}

void Particule::affiche_court(std::ostream& sortie) const { // pour exerciceP9
    marqueur_suivi(sortie);
    sortie << "pos : " << position << " ; v : " << vitesse << endl; // on affiche pas la masse ici
}

void Particule::dessine_sur(SupportADessin &support) const {
    support.dessine(*this);
    if(suivi != nullptr) suivi->dessine_sur(support);
}

// Utile pour les tests
Vecteur3D Particule::quantite_de_mouvement() const {
    return get_masse_reelle() * vitesse;
}

double Particule::energie_cinetique() const {
    return get_masse_reelle() * vitesse.norme2() / 2;
}

// Fais avancer la particule de la distance parcourue pendant le temps dt
void Particule::evolue(double dt, SupportADessin& support) {
    position += vitesse * dt / Vecteur3D::unite_espace; // convertion car vitesse en m/s
    if(suivi != nullptr) suivi->evolue(dt, support);
}


void Particule::echange_chaleur(double coef) {
   vitesse *= coef;
}

/*
 * ON N'UTILISE PLUS CETTE FONCTION CAR ON A DECIDE DE CHANGER LA CONVENTION DES NUMÉROS DE FACES
 *
 * Donne la face correspondante à un rebond
 * (Cette fonction est utilisé pour correspondre à l'affichage de l'énoncé mais pourrait
 * être évité avec une autre convention des numéros des faces. (a-t-on le droit ?)
 * Coord : la coordonnée selon lequel le plan de rebond est défini
 * loin : true si la face est celle éloignée de l'origine
 * exemple : Coord_X et false : plan x = 0
 * Coord_Y et true : plan y = profondeur
 *
 */
//unsigned int Particule::get_face(Coord coord, bool loin) {
//    if(coord == COORD_X and loin) return 4;
//    if(coord == COORD_X and !loin) return 3;
//    if(coord == COORD_Y and loin) return 1;
//    if(coord == COORD_Y and !loin) return 2;
//    if(coord == COORD_Z and loin) return 5;
//    if(coord == COORD_Z and !loin) return 6;
//   throw "Face impossible pour get_face()"; // impossible
//}

// Modifie les vitesses de 2 particules qui s'entrechoquent
void Particule::modifier_vitesse_choc(Particule& p, GenerateurAleatoire& generateur) {
    double M(masse + p.masse);
    Vecteur3D vg(masse/M * vitesse + p.masse/M * p.vitesse);
    Vecteur3D v0(Vecteur3D::aleatoire_norme_fixe(generateur, (vitesse - vg).norme()));
    vitesse = vg + v0;
    p.vitesse = vg - masse/p.masse * v0;
}

void Particule::marqueur_suivi(ostream& sortie) const {
    if(suivi != nullptr) sortie << "* ";
}

//  Opérateurs
ostream& operator<<(ostream& sortie, const Particule& p) {
    return p.affiche(sortie);
}


// Sous classes
// Argon
Argon::Argon(const Vecteur3D& position, const Vecteur3D & vitesse, bool est_suivi) : Particule(position, vitesse, masse_argon, est_suivi) {}

ostream& Argon::affiche(ostream& sortie) const {
    marqueur_suivi(sortie);
    sortie << "particule Argon : ";
    return Particule::affiche(sortie);
}

void Argon::dessine_sur(SupportADessin &support) const {
    support.dessine(*this);
    if(suivi != nullptr) suivi->dessine_sur(support);
}

// Neon
Neon::Neon(const Vecteur3D& position, const Vecteur3D & vitesse, bool est_suivi) : Particule(position, vitesse, masse_neon, est_suivi) {}

std::ostream& Neon::affiche(std::ostream& sortie) const {
    marqueur_suivi(sortie);
    sortie << "particule Neon : ";
    return Particule::affiche(sortie);
}

void Neon::dessine_sur(SupportADessin& support) const {
    support.dessine(*this);
    if(suivi != nullptr) suivi->dessine_sur(support);
}

// Helium
Helium::Helium(const Vecteur3D& position, const Vecteur3D & vitesse, bool est_suivi) : Particule(position, vitesse, masse_helium, est_suivi) {}

ostream& Helium::affiche(ostream& sortie) const {
    marqueur_suivi(sortie);
    sortie << "particule Helium : ";
    return Particule::affiche(sortie);
}

void Helium::dessine_sur(SupportADessin &support) const {
    support.dessine(*this);
    if(suivi != nullptr) suivi->dessine_sur(support);
}


// Suivi
Suivi::Suivi(Particule& particule_) : particule(particule_) {}

void Suivi::evolue(double dt __attribute__((unused)), SupportADessin & support __attribute__((unused))) {
    Vecteur3D position(particule.get_position());
    if(trajectoire.empty() or (position - trajectoire.back()).norme2() > distance_min) trajectoire.push_back(position);
    if(trajectoire.size() > max_points) trajectoire.pop_front();
}

void Suivi::dessine_sur(SupportADessin &support) const {
    support.dessine(*this);
}
