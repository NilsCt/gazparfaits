#include <iostream>
#include "Systeme.h"
#include "Particule.h"
#include "Vecteur3D.h"
using namespace std;

/*
 * Cas du double choc :
 * Si une particule légère fonce dans une particule lourde immobile, la vitesse de celle-ci n'évoluera presque pas
 * Il y aura donc le premier choc de la particule légère sur la particule lourde, puis le choc de la particule lourde
 * sur la particule légère. (car la particule lourde ne bouge presque pas et reste donc dans le rayon de choc de la particule légère)
 */


// tests additionnels (à exerciceP9.cc) sur l'évolution d'un système
int main() {
    ostream& flot(cout);

    // Différents scénarios à tester

    Systeme systeme1; // choc de 2 particules
    systeme1.ajouter_particule(make_unique<Argon>(Vecteur3D(3, 3, 3), Vecteur3D(1, 1, 1)));
    systeme1.ajouter_particule(make_unique<Argon>(Vecteur3D(5, 5, 5), Vecteur3D(0, 0, 0)));
    systeme1.simulation(flot);


    //Systeme systeme2; // rebond d'une particule dans un coin
    //systeme2.ajouter_particule(make_unique<Helium>(Vecteur3D(18, 18, 18), Vecteur3D(1, 1, 1)));
    //systeme2.simulation(flot);

    //Systeme systeme3; // choc d'une particule légère sur une particule lourde immobile
    //systeme3.ajouter_particule(make_unique<Neon>(Vecteur3D(19, 19, 19), Vecteur3D(0, 0, 0)));
    //systeme3.ajouter_particule(make_unique<Helium>(Vecteur3D(15, 15, 15), Vecteur3D(2, 2, 2)));
    //systeme3.simulation(flot);

    //Systeme systeme4; // choc de particules opposées
    //systeme4.ajouter_particule(make_unique<Neon>(Vecteur3D(10, 8, 5), Vecteur3D(0, 1, 0)));
    //systeme4.ajouter_particule(make_unique<Neon>(Vecteur3D(10, 12, 5), Vecteur3D(0, -1, 0)));
    //systeme4.simulation(flot);

    //Systeme systeme5; // choc d'une particule lourde sur une particule légère et éjection en dehors de l'enceinte
    //systeme5.ajouter_particule(make_unique<Neon>(Vecteur3D(15, 15, 15), Vecteur3D(2, 2, 2)));
    //systeme5.ajouter_particule(make_unique<Helium>(Vecteur3D(19, 19, 19), Vecteur3D(0, 0, 0)));
    //systeme5.simulation(flot);

    //Systeme systeme6; // choc de trois particules
    //systeme6.ajouter_particule(make_unique<Argon>(Vecteur3D(10, 8, 10), Vecteur3D(0, 1, 0)));
    //systeme6.ajouter_particule(make_unique<Argon>(Vecteur3D(10, 10, 8), Vecteur3D(0, 0, 1)));
    //systeme6.ajouter_particule(make_unique<Argon>(Vecteur3D(8, 10, 10), Vecteur3D(1, 0, 0)));
    //systeme6.simulation(flot);

    // on pourrait éviter le copier-coller en faisait une liste de listes de particules (mais plus compliqué)
    
    return 0;
}
