#include <iostream>
#include <fstream>
#include "FileWriter.h"
using namespace std;

// Constructeurs
FileWriter::FileWriter(const string& path) : TextViewer(fichier), fichier(path) {
    if(fichier.fail()) {
        cerr << "Erreur lors de l'ouverture du fichier pour FileWriter : " << path << endl;
        throw "Erreur ouverture du fichier " + path; // je mets aussi cerr car le throw affiche le type de l'erreur (string) plutôt que le contenu du string
    }
    // si tout se passe bien :
    cout << "Ouverture du fichier " << path << " réalisée avec succès" << endl;
}

FileWriter::~FileWriter() {
    fichier.close(); // on n'oublie pas de fermer le fichier
    cout << "Fermeture du fichier" << endl;
}

std::ostream &FileWriter::get_flot() const {
    return sortie;
}
