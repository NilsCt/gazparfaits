#include <iostream>
#include <memory>
#include "Systeme.h"
#include "TextViewer.h"
#include "Particule.h"
#include "Vecteur3D.h"
using namespace std;

// Tests associés aux chocs des particules contre l'enceinte et avec d'autres particules
int main() {
    /*
     * Consignes : 
     * Pour faire ce test et avoir exactement le même affichage que l'énoncé,
     * il faut forcer la génération de la direction aléatoire des vecteurs vitesses après les chocs.
     * Il faut donc aller dans Vecteur3D.cc puis décommenter la partie EXERCICE9 (et commenter la fonction classique)
     * ctrl + f "EXERCICE9" peut aider à trouver
     */
     
     /*
      * Remarque : 
      * Pourquoi affiche-t-on l'enceinte à chaque évolution ?
      * Dessiner le système doit dessiner l'enceinte qui est un de ses attributs.
      * Par exemple cela est utile pour l'affichage graphique.
      * De plus, une de nos extensions permet de faire des compressions.
      * Les dimensions de l'enceinte ne sont plus constantes, il est donc pertinent
      * d'afficher leur évolution au cours du temps.
      * (Nous avons décider de ne pas gérer de cas particulier pour ce test
      * qui compliquerait inutilement notre code)
      */
    Systeme systeme;
    ostream& flot(cout); // pour changer facilement le flot
    TextViewer textViewer(flot);
    systeme.ajouter_particule(make_unique<Helium>(Vecteur3D(1,1,1), Vecteur3D(0,0,0)));
    systeme.ajouter_particule(make_unique<Neon>(Vecteur3D(1, 18.5, 1), Vecteur3D(0, 0.2, 0)));
    systeme.ajouter_particule(make_unique<Argon>(Vecteur3D(1, 1, 3.1), Vecteur3D(0, 0, -0.5)));
    flot << "Le système est constitué des 3 particules suivantes :" << endl << endl;
    systeme.dessine_sur(textViewer);

    // On n'utilise pas la méthode simulation() de la classe système pour avoir un affichage qui ressemble à celui de l'énoncé

    flot << endl << "Lancement de la simulation" << endl;
    for(int i(0); i < 10; ++i) {
        flot << "==========----------" << endl;
        systeme.evolue(1e-10, textViewer); // 1 ps = 1e-12 s 
        /*
         * Pourquoi 1e-10 ?
         * Notre méthode évolue prend en paramètre un temps en seconde.
         * Dans l'énoncé de cet exercice dt = 0.1ps = 1e-13 s
         * Nos vitesses sont en m/s mais l'énoncé les utilise en 1000 m/s
         * Ainsi pour avoir le meme déroulement que l'énoncé on prend un pas de temps de 1e-10s
         */
        systeme.dessine_sur(textViewer);
    }


    return 0;
}
