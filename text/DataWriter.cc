#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "DataWriter.h"

using namespace std;

// Constructeurs
DataWriter::DataWriter(const string& path) : fichier(path), derniere_ecriture(0), points(0) {
    if(fichier.fail()) {
        throw "Erreur ouverture du fichier " + path;
    }
    // si tout se passe bien :
    cout << "Ouverture du fichier " << path << " réalisée avec succès" << endl;
}

DataWriter::~DataWriter() {
    fichier.close();
    cout << "Fermeture du fichier" << endl;
}

void DataWriter::dessine_donnees(const Systeme& systeme) {
    double temps(systeme.get_temps());
    if(points <= delai) {
    	++points;
    	return; // on ignore quelques premiers points
    }
    if(derniere_ecriture + periode_echantillonage > temps) return; // trop tôt
    derniere_ecriture = temps;

    // Écriture au format csv (comma separated values)
    // Format : "temps;température;volume;pression totale;pression partielle 1;pression partielle 2; ..."
    // << std::fixed << setprecision(30) // si besoin d'empecher l'écriture scientifique pour exporter les données
    
    fichier << temps << ";" << systeme.temperature() << ";" << systeme.volume() << ";" << systeme.pression_effective();
    vector<double> pression_partielles(systeme.get_pressions_partielles());
    if(pression_partielles.size() > 1) { // s'il n'y a qu'un seul élément, sa pression est la pression totale
        for(auto p : pression_partielles) {
            fichier << ";" << p;
        }
    }
    fichier << endl;
}
