#include <iostream>
#include <cmath>
#include "Enceinte.h"
#include "SupportADessin.h"
#include "Vecteur3D.h"
#include "Systeme.h"
using namespace std;


//  Constructeurs
Enceinte::Enceinte(double h, double l, double p) : hauteur(h), largeur(l), profondeur(p) {}

Enceinte::Enceinte() : Enceinte(20, 20, 20) {}


// Méthodes

/*
 * Renvoie la taille de l'enceinte selon un axe
 * (Voir le schema dans CONCEPTION)
 */
double Enceinte::get_dimension(Coord coord) const {
    switch (coord) {
        case COORD_X: return largeur;
        case COORD_Y: return profondeur;
        case COORD_Z: return hauteur;
        default : throw "Coordonnée impossible pour get_dimension()"; // impossible mais pour éviter warning de geany
    }
}

double Enceinte::volume() const {
    return largeur * profondeur * hauteur * pow(Vecteur3D::unite_espace, 3);
}

double Enceinte::surface() const {
    return 2 * (largeur*profondeur + largeur*hauteur + profondeur*hauteur) * pow(Vecteur3D::unite_espace, 2);
}

Vecteur3D Enceinte::centre() const {
    return Vecteur3D(largeur/2, profondeur/2, hauteur/2);
}

ostream& Enceinte::affiche(ostream& sortie) const {
		return sortie << "hauteur : " << hauteur << " ; largeur : " << largeur << " ; profondeur : " << profondeur << endl;
}

void Enceinte::dessine_sur(SupportADessin &support) const {
    support.dessine(*this);
}

// Opérateurs
ostream& operator<<(ostream& sortie, const Enceinte& enceinte) {
	return enceinte.affiche(sortie);
}

// Enceinte déformable

EnceinteDeformable::EnceinteDeformable(double h_i, double l_i, double p_i, double h_f, double l_f, double p_f, double d) : Enceinte(h_i, l_i, p_i), hauteur_finale(h_f), largeur_finale(l_f), profondeur_finale(p_f), vx((largeur_finale - l_i) / (d*Systeme::time_ratio)), vy((profondeur_finale - p_i) / (d*Systeme::time_ratio)), vz((hauteur_finale - h_i) / (d*Systeme::time_ratio)) {}

bool EnceinteDeformable::compare(double x, double y) {
    	return abs(x - y) < 0.1;
}

void EnceinteDeformable::evolue(double dt, SupportADessin& __attribute__((unused))) {
	if(not compare(largeur, largeur_finale)) {
	    largeur += vx * dt;
	}
	if(not compare(profondeur, profondeur_finale)) {
	    profondeur += vy * dt;
	}
	if(not compare(hauteur, hauteur_finale)) {
	    hauteur += vz * dt;
	}
};

