#include <QApplication>
#include <memory>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"
#include "Comportement.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  GLWidget w(std::make_unique<Systeme>(std::make_unique<EnceinteDeformable>(5,5,5,20,20,20,5), std::make_unique<ComportementDynamique>()));
  Systeme& systeme(w.get_systeme()); // fuite d'encapsulation mais pour initialiser les particules
  // test d'un choc dans une zone créée par l'extension de l'enceinte
  // pour tester l'ajout de nouvelle cellule dans le comportement
  systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(0,0,0), Vecteur3D(11,11,11)));
  systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D(2,2,2), Vecteur3D(10,10,10)));
  w.show();

  return a.exec();
}
