#include <iostream>
#include <memory>
#include "Systeme.h"
#include "DataWriter.h"
#include "Transformation.h"
#include "Comportement.h"
#include "Particule.h"

using namespace std;

int main() {
    Systeme systeme(make_unique<ComportementDynamique>());
    DataWriter dataWriter("donnees.csv");
    systeme.initialiser_particules_precises<Neon>(300, 60);
    cout << "Écriture de la simulation dans le fichier" << endl;
    
    /*
     * ATTENTION : pour avoir des pressions correctes, il est recommandé de changer 
     * max_points_pression à 10 dans Systeme.h
     * (Cette constante est à l'origine à 300 pour lisser les variations de pressions
     * afin d'avoir une valeur plus cohérente)
     * Mais pour tracer des graphiques où la pression évolue vite (compression)
     * il vaut mieux diminuer cette valeur à 10.
     */
    
    
    // Différents scénarios à tester
    
    // Modification de la température pour vérifier la loi des gaz parfait
    //systeme.ajoute_transformation(make_unique<ModificationTemperature>(3000, 80));
    
    // Cycle de Stirling (moteur)
    //systeme.initialiser_cycle_stirling(10, 120, 40000, 320, 20000 );
    
    // Cycle de Carnot (moteur)
    systeme.initialiser_cycle_carnot(17, 260, 64000, 350, 16000 );
 

    for(int i(0); i < 400; ++i) {
        systeme.evolue(0.2*Systeme::time_ratio, dataWriter);
        systeme.dessine_sur(dataWriter);
    }

    return 0;
}
