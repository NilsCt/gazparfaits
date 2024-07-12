#include <QApplication>
#include <memory>
#include <iostream>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"
#include "Comportement.h"
#include "Particule.h"
#include "GenerateurAleatoire.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  GLWidget w(std::make_unique<Systeme>(std::make_unique<ComportementDynamique>()));
  Systeme& systeme(w.get_systeme()); // fuite d'encapsulation, mais pour initialiser les particules
  systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(2, 7, 3), Vecteur3D(4, -3, -5), true));
  systeme.ajouter_particule(std::make_unique<Neon>(Vecteur3D(4, 18, 7), Vecteur3D(-3, 5, 6), true));
  systeme.ajouter_particule(std::make_unique<Helium>(Vecteur3D(6, 14, 12), Vecteur3D(7, -5, 3), true));
  systeme.initialiser_particules(50, 10);
  w.show();

  return a.exec();
}
