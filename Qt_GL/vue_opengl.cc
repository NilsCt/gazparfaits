#include <iostream>
#include <cmath>
#include <array>
#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "SupportADessin.h"
#include "Enceinte.h"
#include "Systeme.h"
#include "Particule.h"
#include "Vecteur3D.h"
#include "glsphere.h"

void VueOpenGL::set_energie_reference(double energie) {
    energie_reference = energie;
}

// couleur d'une particule en fonction de son énergie cinétique
Vecteur3D VueOpenGL::get_couleur(double energie) {
     double d(3*energie_reference - energie_reference);
     Vecteur3D s((energie - energie_reference*3)/(-d) * Vecteur3D(0,0,255) + (energie - energie_reference)/d * Vecteur3D(255,0,0));
     return s;
}

// ======================================================================
void VueOpenGL::dessine_particule(const Particule& particule, Vecteur3D couleur) {
    Vecteur3D pos(particule.get_position());
    QMatrix4x4 matrice;
    matrice.translate(pos.get_x(), pos.get_y(), pos.get_z());
    matrice.scale(0.4);
    if(energie_reference != 0) {
   	couleur = get_couleur(particule.energie_cinetique());
    }
    dessineSphere(matrice, couleur.get_x(), couleur.get_y(), couleur.get_z());
}

void VueOpenGL::dessine(Particule const& particule)
{
    dessine_particule(particule, Vecteur3D(1,1,1));
}

void VueOpenGL::dessine(Neon const& neon)
{
  dessine_particule(neon, Vecteur3D(0,0,1));
}

void VueOpenGL::dessine(Argon const& argon)
{
  dessine_particule(argon, Vecteur3D(0,1,0));
}

void VueOpenGL::dessine(Helium const& helium)
{
  dessine_particule(helium, Vecteur3D(1,0,0));
}

void VueOpenGL::dessine(Enceinte const& enceinte)
{
  QMatrix4x4 matrice;
  matrice.translate(enceinte.get_largeur()/2, enceinte.get_profondeur()/2, enceinte.get_hauteur()/2);
  matrice.scale(10);
  dessineEnceinte(matrice, enceinte);
}

void VueOpenGL::dessine(Systeme const& systeme __attribute__((unused))) {} // normalement impossible d'arriver la

void VueOpenGL::dessine(Suivi const& suivi)
{
    prog.setUniformValue("vue_modele", matrice_vue);
    glBegin(GL_LINE_STRIP);
    for (auto const& point : suivi.get_trajectoire()) {
        glVertex3f(point.get_x(), point.get_y(), point.get_z());
    }
    glEnd();
}

void VueOpenGL::dessine_etat(const Systeme& systeme) const {
    systeme.affiche_etat_thermo(std::cout);
}

void VueOpenGL::replacer_camera(const Systeme& systeme) {
	initializePosition(systeme);
}

// ======================================================================
void VueOpenGL::init(const Systeme& systeme)
{
  /* Initialise notre vue OpenGL.
   * Dans cet exemple, nous créons et activons notre shader.
   *
   * En raison du contenu des fichiers *.glsl, le shader de cet exemple
   * NE permet QUE de dessiner des primitives colorées
   * (pas de textures, brouillard, reflets de la lumière ou autres).
   *
   * Il est séparé en deux parties VERTEX et FRAGMENT.
   * Le VERTEX :
   * - récupère pour chaque sommet des primitives de couleur (dans
   *     l'attribut couleur) et de position (dans l'attribut sommet)
   * - multiplie l'attribut sommet par les matrices 'vue_modele' et
   *     'projection' et donne le résultat à OpenGL
   *   - passe la couleur au shader FRAGMENT.
   *
   * Le FRAGMENT :
   *   - applique la couleur qu'on lui donne
   */

  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

  /* Identifie les deux attributs du shader de cet exemple
   * (voir vertex_shader.glsl).
   *
   * L'attribut identifié par 0 est particulier, il permet d'envoyer un
   * nouveau "point" à OpenGL
   *
   * C'est pourquoi il devra obligatoirement être spécifié et en dernier
   * (après la couleur dans cet exemple, voir plus bas).
   */

  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("couleur", CouleurId);

  // Activation du shader
  prog.bind();

  /* Activation du "Test de profondeur" et du "Back-face culling"
   * Le Test de profondeur permet de dessiner un objet à l'arrière-plan
   * partielement caché par d'autres objets.
   *
   * Le Back-face culling consiste à ne dessiner que les face avec ordre
   * de déclaration dans le sens trigonométrique.
   */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  sphere.initialize();
  initializePosition(systeme);
}

// ======================================================================

double average(double x, double y, double z) {
    //if(x > y) {
    //	if(x > z) return x; else return z;
    //} else {
    //	if(y > z) return y; else return z;
    //}
    return (x + y + z)/3;
}

void VueOpenGL::initializePosition(const Systeme& systeme)
{
  // position initiale
  matrice_vue.setToIdentity();
  matrice_vue.rotate(-90, 1, 0, 0);
  const Enceinte& enceinte(systeme.get_enceinte());
  double largeur(enceinte.get_largeur_max()), hauteur(enceinte.get_hauteur_max()), profondeur(enceinte.get_profondeur_max());
  matrice_vue.translate(-largeur/2, average(largeur, profondeur, hauteur), -hauteur/2); // voir depuis la face y = -1
  //matrice_vue.rotate(45.0, 0.0, 0.0, 1.0);
}

// ======================================================================
void VueOpenGL::translate(double x, double y, double z)
{
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(x, y, z);
  matrice_vue = translation_supplementaire * matrice_vue;
}

// ======================================================================
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}

// ======================================================================
double texture(double x, double malus) {
  double res(x - malus);
  if(res > 0) return res; else return 0;
}

void VueOpenGL::dessineEnceinte (QMatrix4x4 const& point_de_vue, const Enceinte& enceinte)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_LINES);
  double dx(enceinte.get_largeur()/20); // 20 = scale (10) * taille face (2)
  double dy(enceinte.get_profondeur()/20);
  double dz(enceinte.get_hauteur()/20);

  // face coté X = +1
  prog.setAttributeValue(CouleurId, 1, 1, 1);
  prog.setAttributeValue(SommetId, +dx, -dy, -dz);
  prog.setAttributeValue(SommetId, +dx, +dy, -dz);
  prog.setAttributeValue(SommetId, +dx, +dy, +dz);
  prog.setAttributeValue(SommetId, +dx, -dy, +dz);

  // face coté X = -1
  prog.setAttributeValue(SommetId, -dx, -dy, -dz);
  prog.setAttributeValue(SommetId, -dx, -dy, +dz);
  prog.setAttributeValue(SommetId, -dx, +dy, +dz);
  prog.setAttributeValue(SommetId, -dx, +dy, -dz);

  // face coté Y = +1
  prog.setAttributeValue(SommetId, -dx, +dy, -dz);
  prog.setAttributeValue(SommetId, -dx, +dy, +dz);
  prog.setAttributeValue(SommetId, +dx, +dy, +dz);
  prog.setAttributeValue(SommetId, +dx, +dy, -dz);

  // face coté Y = -1
  prog.setAttributeValue(SommetId, -dx, -dy, -dz);
  prog.setAttributeValue(SommetId, +dx, -dy, -dz);
  prog.setAttributeValue(SommetId, +dx, -dy, +dz);
  prog.setAttributeValue(SommetId, -dx, -dy, +dz);

  // face coté Z = +1
  prog.setAttributeValue(SommetId, -dx, -dy, +dz);
  prog.setAttributeValue(SommetId, +dx, -dy, +dz);
  prog.setAttributeValue(SommetId, +dx, +dy, +dz);
  prog.setAttributeValue(SommetId, -dx, +dy, +dz);

  // face coté Z = -1
  prog.setAttributeValue(SommetId, -dx, -dy, -dz);
  prog.setAttributeValue(SommetId, -dx, +dy, -dz);
  prog.setAttributeValue(SommetId, +dx, +dy, -dz);
  prog.setAttributeValue(SommetId, +dx, -dy, -dz);
  
  
  prog.setAttributeValue(SommetId, +dx, -dy, +dz); // rajouter les traits manquants
  prog.setAttributeValue(SommetId, +dx, -dy, -dz);
  
  prog.setAttributeValue(SommetId, -dx, -dy, +dz);
  prog.setAttributeValue(SommetId, -dx, +dy, +dz);
  
  prog.setAttributeValue(SommetId, -dx, +dy, -dz);
  prog.setAttributeValue(SommetId, +dx, +dy, -dz);
  

  glEnd();
}

void VueOpenGL::dessineSphere(QMatrix4x4 const& point_de_vue, double rouge, double vert, double bleu) {
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
    prog.setAttributeValue(CouleurId, rouge, vert, bleu);  // met la couleur
    sphere.draw(prog, SommetId);
}


