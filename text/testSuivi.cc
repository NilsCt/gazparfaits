#include <iostream>
#include <memory>
#include "Systeme.h"
#include "TextViewer.h"
#include "Particule.h"
#include "Vecteur3D.h"
using namespace std;


// Tests associés à la classe Enceinte
int main () {
	Systeme systeme;
     	systeme.initialiser_particules(5, 0.2);
     	systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(2, 7, 3), Vecteur3D(4, -3, -5), true));
     	systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(10, 10, 10), Vecteur3D(1, 0, 0), true));
     	systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(11, 10, 10), Vecteur3D(0, 0, 0)));
     	systeme.ajouter_particule(std::make_unique<Neon>(Vecteur3D(4, 18, 7), Vecteur3D(-3, 5, 6), true));
     	systeme.ajouter_particule(std::make_unique<Helium>(Vecteur3D(6, 14, 12), Vecteur3D(7, -5, 3), true));
     	systeme.simulation(cout);
	return 0;
}
