#pragma once

#include <QOpenGLWidget>        // Classe pour faire une fenêtre OpenGL
#include <QElapsedTimer>        // Classe pour gérer le temps
#include <memory>
#include "vue_opengl.h"
#include "Systeme.h"
#include "glsphere.h"

class GLWidget : public QOpenGLWidget
/* La fenêtre hérite de QOpenGLWidget ;
 * les événements (clavier, souris, temps) sont des méthodes virtuelles à redéfinir.
 */
{
public:
  GLWidget(std::unique_ptr<Systeme> && systeme_, QWidget* parent = nullptr);
  virtual ~GLWidget() = default;
  
  Systeme& get_systeme() { return *systeme; } 
  /*
  Fuite d'encapsulation mais obligé pour ajouter les particules au système sans faire de
  de copie initiale du Système (créer un système puis le donner au GLWidget)
  */ 

  void actualiser_energie_reference();

private:

  // Les 3 méthodes clés de la classe QOpenGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event)  override;

  // Méthodes de gestion interne
  void pause();

  // Vue : ce qu'il faut donner au contenu pour qu'il puisse se dessiner sur la vue
  VueOpenGL vue;

  // Timer
  int timerId;
  // pour faire évoluer les objets avec le bon "dt"
  QElapsedTimer chronometre;
  
  QPoint lastMousePosition;

  // objets à dessiner, faire évoluer
  std::unique_ptr<Systeme> systeme;
};
