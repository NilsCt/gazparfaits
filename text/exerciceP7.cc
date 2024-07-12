#include <iostream>
#include "TextViewer.h"
#include "Particule.h"
#include "Vecteur3D.h"
using namespace std;

int main() {
    TextViewer textViewer(cout);
    Neon neon(Vecteur3D(1, 18.5, 1), Vecteur3D(0, 0.2, 0));
    Argon argon(Vecteur3D(1, 1, 3.1), Vecteur3D(0, 0, -0.5));
    textViewer.dessine(neon);
    cout << endl;
    textViewer.dessine(argon);
    return 0;
}
