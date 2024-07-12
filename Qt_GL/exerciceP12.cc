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
  Systeme& systeme(w.get_systeme());
  
  systeme.initialiser_particules(20, 0.1);
  systeme.initialiser_particules_precises<Helium>(5, 10);
  
  //systeme.initialiser_particules(150, 10);
  //systeme.initialiser_particules_precises<Neon>(50, 20);
  //systeme.initialiser_particules_precises<Argon>(30, 50);
  //systeme.initialiser_particules_precises<Helium>(20, 15);
  
   w.show();

  return a.exec();
}
