#include <QApplication>
#include <memory>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  GLWidget w(std::make_unique<Systeme>(std::make_unique<EnceinteDeformable>(
  	20,20,20,10,10,10,15)));
  Systeme& systeme(w.get_systeme()); // fuite d'encapsulation mais pour initialiser les particules
  systeme.initialiser_particules(30, 0.2);
  w.show();

  return a.exec();
}
