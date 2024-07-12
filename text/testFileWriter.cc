#include <iostream>
#include <memory>
#include "FileWriter.h"
#include "Systeme.h"
#include "Particule.h"
#include "Vecteur3D.h"
using namespace std;

// Tests associés à la class FileWriter
int main() {
    FileWriter fileWriter("evolution.txt");
    Systeme systeme;
    systeme.ajouter_particule(make_unique<Neon>(Vecteur3D(10, 8, 5), Vecteur3D(0, 1, 0)));
    systeme.ajouter_particule(make_unique<Neon>(Vecteur3D(10, 12, 5), Vecteur3D(0, -1, 0)));
    systeme.ajouter_particule(make_unique<Argon>(Vecteur3D(5, 3, 16), Vecteur3D(-1, -2, -2)));
    systeme.ajouter_particule(make_unique<Helium>(Vecteur3D(9, 17, 8), Vecteur3D(-1, 0, -1)));
    cout << "Écriture de la simulation dans le fichier" << endl;
    systeme.simulation(fileWriter.get_flot());
    return 0;
}
