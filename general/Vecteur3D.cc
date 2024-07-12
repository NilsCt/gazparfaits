#include <iostream>
#include <cmath>
#include "Vecteur3D.h"
using namespace std;


// ===================================
//  Constructeurs
// ===================================
/*
 * Constructeur par défaut qui donne le vecteur nul
 * On utilise ce constructeur par défaut plutôt qu'un constructeur avec des paramètres par défaut
 * pour empêcher la construction de vecteur Vecteur3D(7) ou Vecteur3D(7, 5) qui sont ambigus
 */
Vecteur3D::Vecteur3D() : x(0), y(0), z(0) {}

Vecteur3D::Vecteur3D(double x, double y, double z) : x(x), y(y), z(z) {}


// Factory
/*
 * Génère un vecteur d'une certaine norme avec une direction aléatoire choisie selon
 * une distribution uniforme
 */
Vecteur3D Vecteur3D::aleatoire_norme_fixe(GenerateurAleatoire & generateur, double norme_) {
    double z(generateur.uniforme(-norme_, norme_));
    double phi(generateur.uniforme(0, 2*M_PI));
    double r(sqrt(norme_*norme_ - z*z));
    return Vecteur3D(r * cos(phi), r * sin(phi), z); // tant pis pour la copie (petit objet)
}

//// EXERCICE9 décommenter ici et commenter la méthode juste au dessus
//Vecteur3D Vecteur3D::aleatoire_norme_fixe(GenerateurAleatoire & generateur, double norme_) {
//    double z(0);
//    double phi(M_PI/3);
//    double r(sqrt(norme_*norme_ - z*z));
//    return Vecteur3D(r * cos(phi), r * sin(phi), z);
//}

// Génère un vecteur aléatoire avec des composantes suivant une loi gaussienne (moy 0, variance précisée)
Vecteur3D Vecteur3D::aleatoire_gaussienne(GenerateurAleatoire& generateur, double variance) {
    double x(generateur.gaussienne(0.0, sqrt(variance)));
    double y(generateur.gaussienne(0.0, sqrt(variance)));
    double z(generateur.gaussienne(0.0, sqrt(variance)));
    return Vecteur3D(x,y,z);
}

// Génère un vecteur aléatoire avec des composantes suivant une loi uniforme (min 0, max précisé)
Vecteur3D Vecteur3D::aleatoire_uniforme(GenerateurAleatoire& generateur, double max_x, double max_y, double max_z) {
    double x(generateur.uniforme(0, max_x));
    double y(generateur.uniforme(0, max_y));
    double z(generateur.uniforme(0, max_z));
    return Vecteur3D(x, y, z);
}


// ===================================
//  Méthodes
// ===================================

double Vecteur3D::get_x() const { return x; }

double Vecteur3D::get_y() const { return y; }

double Vecteur3D::get_z() const { return z; }

double Vecteur3D::get_coord(Coord coord) const {
    switch (coord) {
        case COORD_X: return x;
        case COORD_Y: return y;
        case COORD_Z: return z;
        default: throw "Coordonnée impossible pour get_coord()"; // impossible mais pour éviter warning de geany
    }
}

// Affiche un vecteur dans un stream
ostream& Vecteur3D::affiche(std::ostream& sortie) const {
    return sortie  << x << " " << y << " " << z;
}

// Modifie une coordonnée d'un vecteur
void Vecteur3D::set_coord(Coord coord, double value) {
    switch (coord) {
        case COORD_X: x = value; break; // choix de la coordonnée à définir
        case COORD_Y: y = value; break;
        case COORD_Z: z = value; break;
    }
}

// Modifie toutes les coordonnées du vecteur
Vecteur3D& Vecteur3D::set_all_coord(double i, double j, double k) {
    x = i;
    y = j;
    z = k;
    return *this;
}

// Norme au carré du vecteur
double Vecteur3D::norme2() const {
    return (*this * *this); // (utilise le produit scalaire)
}

// Norme du vecteur
double Vecteur3D::norme() const {
    return sqrt(norme2());
}

// Angle avec un autre vecteur (en radian)
double Vecteur3D::angle(const Vecteur3D& v) const {
    return acos( (*this * v) / (norme() * v.norme()) );
}

/*
 * Oppose la coordonnée fournie
 * (Revient à modifier le vecteur en sa symétrie par rapport à un axe)
 * Voir fichier CONCEPTION pour un schéma
 */
void Vecteur3D::coord_oppose(Coord coord) {
    switch (coord) {
        case COORD_X: x = -x; break;
        case COORD_Y: y = -y; break;
        case COORD_Z: z = -z; break;
    }
}

// Donne le vecteur d'entier correspondant après pavage (échantillonnage) de l'espace
Vecteur3D Vecteur3D::pavage(double epsilon) const {
    return Vecteur3D(floor(x/epsilon), floor(y/epsilon), floor(z/epsilon));
}

// ===================================
//  Opérateurs
// ===================================

// Affiche un vecteur dans un ostream
ostream& operator<<(ostream& sortie, const Vecteur3D& v) {
    return v.affiche(sortie);
}

// Compare le vecteur avec un autre donné paramètre
bool Vecteur3D::operator==(const Vecteur3D& v) const {
    // La précision est une variable static associée à la classe Vecteur3D
    return abs(x - v.x) < precision and abs(y - v.y) < precision and abs(z - v.z) < precision;
}

bool Vecteur3D::operator!=(const Vecteur3D& v) const {
    return not (*this == v);
}

// Addition avec un autre vecteur
Vecteur3D& Vecteur3D::operator+=(const Vecteur3D& v) {
    return set_all_coord(x + v.x, y + v.y, z + v.z);
}

const Vecteur3D operator+(Vecteur3D v1, const Vecteur3D& v2) {
    return v1 += v2;
}

// Multiplication par un scalaire le vecteur
Vecteur3D& Vecteur3D::operator*=(double k) {
    return set_all_coord(k * x, k * y, k * z);
}

Vecteur3D& Vecteur3D::operator/=(double k) {
    return (*this *= 1/k);
}

const Vecteur3D operator*(Vecteur3D v, double k) {
    return v *= k;
}

const Vecteur3D operator*(double x, const Vecteur3D& v) {
    return v * x;
}

const Vecteur3D operator/(Vecteur3D v, double k) {
    return v /= k;
}

// Opposé du vecteur
const Vecteur3D operator-(Vecteur3D v) {
    return v * -1;
}

// Soustraction par un autre vecteur
Vecteur3D& Vecteur3D::operator-=(const Vecteur3D& v) {
    return *this += -v;
}

const Vecteur3D operator-(Vecteur3D v1, const Vecteur3D& v2) {
    return v1 -= v2;
}

// Produit vectoriel avec un autre vecteur
Vecteur3D& Vecteur3D::operator^=(const Vecteur3D& v) {
    return set_all_coord(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
    );
}

const Vecteur3D operator^(Vecteur3D v1, const Vecteur3D& v2) {
    return v1 ^= v2;
}

// Produit scalaire avec un autre vecteur
double Vecteur3D::operator*(const Vecteur3D& v) const {
    return x * v.x + y * v.y + z * v.z;
}

/*
 * Vecteur unitaire (norme = 1) de même direction et même sens que le vecteur
 * Lance une erreur s'il s'agit du vecteur nul
 */
const Vecteur3D Vecteur3D::operator~() const {
    double s(norme()); // pour n'appeler qu'une seule fois la méthode norme
    if(s == 0) { // le vecteur nul n'a pas de direction
        throw "Le vecteur nul n'a pas de vecteur unitaire";
    }else {
        return Vecteur3D(*this) *= 1/s;
    }
}
