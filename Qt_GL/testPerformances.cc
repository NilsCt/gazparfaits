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
  GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(100,100,100),std::make_unique<ComportementDynamique>()));
  Systeme& systeme(w.get_systeme());
  systeme.initialiser_particules(1000, 50);
  w.show();

  return a.exec();
}
