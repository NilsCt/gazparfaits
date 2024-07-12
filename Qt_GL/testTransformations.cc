#include <QApplication>
#include <memory>
#include <iostream>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"
#include "Comportement.h"
#include "Particule.h"
#include "GenerateurAleatoire.h"
#include "Transformation.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    
    /*
     * Ordre des paramètres :
     * Transformation de l'enceinte (travail) : dimension de l'enceinte finale, durée
     * Modification température : température finale, durée
     * Cycle : durée pour chaque étape (la même pour toute), T_a, V_a, T_c, V_c (avec A et C 2 points opposés dans le cycle)
     */


    // Différentes transformations à tester
   
    // Enchainement de transformations de base
    //GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(30,30,30),std::make_unique<ComportementDynamique>()));
    //Systeme& systeme(w.get_systeme());
    //systeme.initialiser_particules(500, 15);
    //systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(50,50,50, 10));
    //systeme.ajoute_transformation(std::make_unique<ModificationTemperature>(25, 10));
    //systeme.ajoute_transformation(std::make_unique<TravailAdiabatique>(20,20,20, 10));
    //systeme.ajoute_transformation(std::make_unique<Pause>(5));
    //systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(55,55,55, 10));
    //systeme.ajoute_transformation(std::make_unique<ModificationTemperature>(25, 10));
    //systeme.ajoute_transformation(std::make_unique<TravailAdiabatique>(40,40,40, 10));


    // Cycle de Stirling (moteur)
    //GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(40,40,40),std::make_unique<ComportementDynamique>()));
    //Systeme& systeme(w.get_systeme());
    //systeme.initialiser_particules(500, 25);
    //w.actualiser_energie_reference(); // la couleur des particules dépend de leur énergie cinétique
    //systeme.initialiser_cycle_stirling(10,5,64000,300,8000);
    
     
    // Cycle de Carnot (moteur)
    //GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(40,40,40),std::make_unique<ComportementDynamique>()));
    //Systeme& systeme(w.get_systeme());
    //systeme.initialiser_particules(500, 100);
    //w.actualiser_energie_reference();
    //systeme.initialiser_cycle_carnot(10, 100, 64000, 300, 8000 );
    
    
    // Modifications de l'enceinte asymétriques
    GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(40,40,40),std::make_unique<ComportementDynamique>()));
    Systeme& systeme(w.get_systeme());
    systeme.initialiser_particules(500, 30);
    for(int i(0); i < 50; ++i) {
        systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(10,40,40, 5));
        systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(40,10,40, 5));
        systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(40,40,10, 5));
    }
    
    // Dissipation de la chaleur
    //GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(40,40,40),std::make_unique<ComportementDynamique>()));
    //Systeme& systeme(w.get_systeme());
    //systeme.initialiser_particules(500, 300);
    //systeme.ajoute_transformation(std::make_unique<ModificationTemperature>(1e-5, 30));
    
    // Implosion de l'enceinte
    //GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(40,40,40),std::make_unique<ComportementDynamique>()));
    //Systeme& systeme(w.get_systeme());
    //systeme.initialiser_particules(500, 100);
    //systeme.ajoute_transformation(std::make_unique<TravailIsotherme>(0,0,0, 10));
    

    w.show();

    return a.exec();
}
