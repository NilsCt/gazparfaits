#include <iostream>
#include <memory>
#include "Systeme.h"
#include "Enceinte.h"
using namespace std;


// Tests associés à la classe Enceinte
int main () {
	Systeme systeme(make_unique<EnceinteDeformable>(20,20,20,10,10,10,30));
     	systeme.initialiser_particules(30, 0.2);
     	systeme.simulation(cout); // fait évoluer 10*time_ratio s à la fois
     	// il faut donc mettre une durée de transformation d'au moins 10 (ici 30)
	return 0;
}
