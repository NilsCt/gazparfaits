#pragma once

#include <QOpenGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include <array>
#include "SupportADessin.h"
#include "Enceinte.h"
#include "Systeme.h"
#include "Particule.h"
#include "Vecteur3D.h"
#include "glsphere.h"

class VueOpenGL : public SupportADessin {
 public:
 
    virtual void dessine(Enceinte const&) override;
    virtual void dessine(Systeme const&) override;
    virtual void dessine(Particule const&) override;
    virtual void dessine(Neon const&) override;
    virtual void dessine(Argon const&) override;
    virtual void dessine(Helium const&) override;
    virtual void dessine(Suivi const&) override;

    virtual void dessine_etat(const Systeme& systeme) const override;
    
    virtual void replacer_camera(const Systeme& systeme) override;

  // méthodes de (ré-)initialisation
  void init(const Systeme& systeme);
  void initializePosition(const Systeme& systeme);

  // méthode set
  void setProjection(QMatrix4x4 const& projection)
  { prog.setUniformValue("projection", projection); }

  // Méthodes set
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);
  
  // méthode utilitaire offerte pour simplifier
  //void dessineCube(QMatrix4x4 const& point_de_vue, bool plein, double r, double g, double b );
  void dessineEnceinte(QMatrix4x4 const& point_de_vue, const Enceinte& enceinte);
  void dessineSphere(QMatrix4x4 const& point_de_vue, double rouge = 1.0, double vert = 1.0, double bleu = 1.0);

  void set_energie_reference(double energie);

 private:
 
     void dessine_particule(const Particule& particule, Vecteur3D couleur);
     // pas de référence constante pour couleur fait exprès
 
  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;
  GLSphere sphere;

  // Caméra
  QMatrix4x4 matrice_vue;
  
  double energie_reference = 0;
  
  // couleur d'une particule en fonction de son énergie cinétique
  Vecteur3D get_couleur(double energie);
};
