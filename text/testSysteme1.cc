#include <iostream>
#include "Systeme.h"
#include "Enceinte.h"
#include "Particule.h"
#include "TextViewer.h"
using namespace std;

// Tests associés à la classe Système et TextViewer
int main() {
    ostream& flot(cout);
    Systeme systeme1(10,20,30);
    TextViewer textViewer(flot);

    systeme1.ajouter_particule(make_unique<Helium>(Vecteur3D(1, 1, 1),  Vecteur3D(0, 0, 0)));
    systeme1.ajouter_particule(make_unique<Neon>(Vecteur3D(1, 18.5, 1),  Vecteur3D(0, 0.2, 0)));
    systeme1.ajouter_particule(make_unique<Argon>(Vecteur3D(1, 1, 3.1),  Vecteur3D(0, 0, -0.5)));
    systeme1.affiche_detaille(cout);
    
    // Tests supplémentaires
    flot << endl;
    systeme1.dessine_sur(textViewer);
    flot << endl;

    systeme1.vider();
    flot << systeme1 << endl; // n'affiche rien exprès

    Systeme systeme2;
    systeme2.affiche_detaille(flot);
    flot << endl;

    Enceinte enceinte(10,20,30);
    enceinte.dessine_sur(textViewer);
    flot << endl;

    Particule particule(Vecteur3D(1,1,1), Vecteur3D(0,0,0), 5.2);
    Neon neon(Vecteur3D(2,2,2), Vecteur3D(0,0,0));
    Argon argon(Vecteur3D(3,3,3), Vecteur3D(0,0,0));
    particule.dessine_sur(textViewer);
    neon.dessine_sur(textViewer);
    argon.dessine_sur(textViewer);
    return 0;
}
