#include <iostream>
#include <vector>
#include <iomanip>
#include "Particule.h"
#include "Vecteur3D.h"

using namespace std;

// Tests associés à la classe Particule
int main () { 
	Particule part1(Vecteur3D(1, 1, 1),  Vecteur3D(0, 0, 0), 4.002602);
	Particule part2(Vecteur3D(1, 18.5, 1),  Vecteur3D(0, 0.2, 0), 20.1797);
	Particule part3(Vecteur3D(1, 1, 3.1),  Vecteur3D(0, 0 , -0.5), 39.948);
	Particule part4(part2);
	vector<Particule> particules({part1, part2, part3, part4});


    for (size_t i(0); i < particules.size(); ++i) {
        cout << "particule " << i + 1 << " : " << particules[i];
    }
}
