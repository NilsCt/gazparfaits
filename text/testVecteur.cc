#include <iostream>
#include "Vecteur3D.h"
using namespace std;

// Fonction utilitaire pour raccourcir le code de l'affichage des tests
void affiche_operation(char op, const Vecteur3D& u, const Vecteur3D& v) {
    Vecteur3D res(u);
    switch (op) {
        case '+': res += v ; break;
        case '-': res -= v; break;
        case '^':  res ^= v; break;
        default:; // éviter warning
    }
    cout << u << "  " << op << "  " << v << "  =  " << res << endl;
}

int main() {
    Vecteur3D vect1(1,2,-0.1), vect2(2.6, 3.5, 4.1), vect3(vect1), nul;

    cout << "Vecteur 1 : " << vect1 << endl;
    cout << "Vecteur 2 : " << vect2 << endl;
    cout << "Vecteur 3 : " << vect3 << endl;
    cout << "Vecteur 4 : " << nul << endl;

    cout << "Le vecteur 1 est " << (vect1 == vect2 ? "égal au" : "différent du");
    cout << " vecteur 2," << endl << "et est ";
    cout << (vect1 != vect3 ? "différent du" : "égal au") << " vecteur 3." << endl; // test pour != exprès

    affiche_operation('+',vect1, vect2);
    affiche_operation('+',vect2, vect1);
    affiche_operation('+', vect1, nul);
    affiche_operation('+', nul, vect1);
    affiche_operation('-', vect1, vect2);
    affiche_operation('-', vect2, vect2);
    cout << " - (" << vect1 << ")  =  " << -vect1 << endl;
    affiche_operation('+',-vect2, vect1);
    affiche_operation('+', vect1, -vect1);
    cout << vect1 << "  * 3 " << "  =  " << vect1 * 3 << endl;
    cout << "3 *  " << vect1 << "  =  " << 3 * vect1 << endl;
    cout << vect1 << "  *  " << vect2 << "  =  " << vect1 * vect2 << endl;
    cout << vect2 << "  *  " << vect1 << "  =  " << vect1 * vect2 << endl;
    affiche_operation('^', vect1, vect2);
    affiche_operation('^', vect2, vect1);
    cout << "||" << vect1 << "||^2 =  " << vect1.norme2() << endl;
    cout << "||" << vect1 << "|| =  " << vect1.norme() << endl;
    cout << "||" << vect2 << "||^2  = " << vect2.norme2() << endl;
    cout << "||" << vect2 << "|| =  " << vect2.norme() << endl;
    Vecteur3D unit(~vect1); // vecteur unitaire
    cout << "Unitaire :  " << unit << endl;
    try {
        cout << "Unitaire :  " << ~nul << endl;
    }catch (...) {
        cout << "Erreur planifiée : pas de vecteur unitaire pour le vecteur nul" << endl;
    }
    cout << "Norme unitaire :  " << unit.norme() << endl;
    cout << "Produit scalaire avec vecteur unitaire :  " << vect1 << "  *  " << unit << "  =  " << vect1 * unit << endl; // redonne la norme
    cout << "Produit vectoriel avec vecteur unitaire : ";
    affiche_operation('^',vect1, unit);
    affiche_operation('^',vect1, nul);
    cout << "Angle entre v1 et v2 =  " << vect1.angle(vect2) << endl;
    cout << "Angle entre v2 et v1 =  " << vect2.angle(vect1) << endl;
    cout << "Angle entre v1 et v1 =  " << vect1.angle(vect1) << endl;

    cout << "v1 =  " << vect1 << endl;
    cout << "v1 += v2 =  " << (vect1 += vect2) << endl;
    cout << "v1 -= v2 =  " << (vect1 -= vect2) << endl;
    cout << "v1 *= 5 =  " << (vect1 *= 5) << endl;
    cout << "v1 ^= v2 =  " << (vect1 ^= vect2) << endl;
    return 0;
}
