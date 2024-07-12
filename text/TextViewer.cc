#include <iostream>
#include "TextViewer.h"
#include "Particule.h"
#include "Enceinte.h"
#include "Systeme.h"
using namespace std;

// Constructeurs
TextViewer::TextViewer(ostream& sortie) : SupportADessin(), sortie(sortie) {}


// Méthodes
void TextViewer::dessine(const Particule& p) { // l'énoncé nous force à définir toutes ces méthodes (on ne peut pas polymorphisme ?!)
    sortie << p;
}

void TextViewer::dessine(const Argon& p) {
    sortie << p;
}

void TextViewer::dessine(const Neon& p) {
    sortie << p;
}

void TextViewer::dessine(const Helium& p) {
    sortie << p;
}

void TextViewer::dessine(const Enceinte& e) {
    sortie << e;
}

void TextViewer::dessine(const Systeme& s) {
    sortie << s;
}

void TextViewer::dessine_rebond(size_t index, unsigned int face) {
    sortie << "La particule " << index << " rebondit sur la face " << face << endl;
}

void TextViewer::dessine_avant_choc(const Particule& p1, const Particule& p2, size_t index) {
    sortie << "La particule " << index << " entre en collision avec une autre particule." << endl;
    sortie << " avant le choc :" << endl;
    sortie << "   part. " << index << " : : ";
    p1.affiche_court(sortie); // on ne veut pas afficher la masse comme dans l'énoncé exerciceP9
    sortie << "   autre   : : ";
    p2.affiche_court(sortie);
}

void TextViewer::dessine_apres_choc(const Particule& p1, const Particule& p2, size_t index) {
    sortie << " apres le choc :" << endl;
    sortie << "   part. " << index << " : : ";
    p1.affiche_court(sortie);
    sortie << "   autre   : : ";
    p2.affiche_court(sortie);
}

void TextViewer::dessine_etat(const Systeme& systeme) const {
    systeme.affiche_etat_thermo(sortie);
}

