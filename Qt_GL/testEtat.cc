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
  GLWidget w(std::make_unique<Systeme>(std::make_unique<Enceinte>(50,50,50),std::make_unique<ComportementDynamique>()));
  Systeme& systeme(w.get_systeme()); // fuite d'encapsulation, mais pour initialiser les particules
   systeme.initialiser_particules(300, 2); // on utilise 2 méthodes pour initialiser les particules pour tester leurs validitées
  GenerateurAleatoire gen;
  for(int i(0); i < 100; ++i) {
  	systeme.ajouter_particule(std::make_unique<Argon>(Vecteur3D::aleatoire_uniforme(gen, 20, 20, 20) , Vecteur3D::aleatoire_norme_fixe(gen, 35))); // normes des vitesses calculées pour 2K
  	//systeme.ajouter_particule(std::make_unique<Neon>(Vecteur3D::aleatoire_uniforme(gen, 20, 20, 20) , Vecteur3D::aleatoire_norme_fixe(gen, 50)));
  	//systeme.ajouter_particule(std::make_unique<Helium>(Vecteur3D::aleatoire_uniforme(gen, 20, 20, 20) , Vecteur3D::aleatoire_norme_fixe(gen, 111)));
  }
  w.show();

  return a.exec();
}
