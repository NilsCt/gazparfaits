#include <QApplication>
#include <memory>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"
#include "Particule.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  GLWidget w(std::make_unique<Systeme>(std::make_unique<ComportementDynamique>()));
  Systeme& systeme(w.get_systeme()); // fuite d'encapsulation mais pour initialiser les particules
  systeme.initialiser_particules(5, 0.2);
  systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(2, 7, 3), Vecteur3D(12, -8, -15), true));
  systeme.ajouter_particule(std::make_unique<Neon>(Vecteur3D(4, 18, 7), Vecteur3D(-8, 15, 18), true));
  systeme.ajouter_particule(std::make_unique<Helium>(Vecteur3D(6, 14, 12), Vecteur3D(20, -15, 8), true));
  systeme.initialiser_particules(50, 8);
  
   w.show();

  return a.exec();
}
