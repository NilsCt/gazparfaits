#include <QKeyEvent>
#include <iostream>
#include <QTimerEvent>
#include <QMatrix4x4>
#include <memory>
#include "glwidget.h"
#include "Systeme.h"
#include "Enceinte.h"
#include "Particule.h"
#include "Vecteur3D.h"
#include "glsphere.h"

using namespace std;


void GLWidget::actualiser_energie_reference() {
   vue.set_energie_reference(systeme->energie_cinetique_moyenne());
}

// ======================================================================
GLWidget::GLWidget(std::unique_ptr<Systeme> && systeme_, QWidget* parent) : QOpenGLWidget(parent), systeme(std::move(systeme_)){
	chronometre.restart();
}

void GLWidget::initializeGL()
{
  vue.init(*systeme);
  timerId = startTimer(20);
}

// ======================================================================
void GLWidget::resizeGL(int width, int height)
{
  /* On commance par dire sur quelle partie de la 
   * fenêtre OpenGL doit dessiner.
   * Ici on lui demande de dessiner sur toute la fenêtre.
   */
  glViewport(0, 0, width, height);

  /* Puis on modifie la matrice de projection du shader.
   * Pour ce faire on crée une matrice identité (constructeur 
   * par défaut), on la multiplie par la droite par une matrice
   * de perspective.
   * Plus de détail sur cette matrice
   *     http://www.songho.ca/opengl/gl_projectionmatrix.html
   * Puis on upload la matrice sur le shader à l'aide de la
   * méthode de la classe VueOpenGL
   */
  QMatrix4x4 matrice;
  matrice.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
  vue.setProjection(matrice);
}

// ======================================================================
void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  systeme->dessine_sur(vue);
}


// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  constexpr double petit_angle(5.0); // en degrés
  constexpr double petit_pas(1.0);

  switch (event->key()) {

  case Qt::Key_Left:
    vue.rotate(petit_angle, 0.0, -1.0, 0.0);
    break;

  case Qt::Key_Right:
    vue.rotate(petit_angle, 0.0, +1.0, 0.0);
    break;

  case Qt::Key_Up:
    vue.rotate(petit_angle, -1.0, 0.0, 0.0);
    break;

  case Qt::Key_Down:
    vue.rotate(petit_angle, +1.0, 0.0, 0.0);
    break;

  case Qt::Key_PageUp:
  case Qt::Key_W:
    vue.translate(0.0, 0.0,  petit_pas);
    break;

  case Qt::Key_PageDown:
  case Qt::Key_S:
    vue.translate(0.0, 0.0, -petit_pas);
    break;

  case Qt::Key_A:
    vue.translate( petit_pas, 0.0, 0.0);
    break;

  case Qt::Key_D:
    vue.translate(-petit_pas, 0.0, 0.0);
    break;

  case Qt::Key_R:
    vue.translate(0.0, -petit_pas, 0.0);
    break;

  case Qt::Key_F:
    vue.translate(0.0,  petit_pas, 0.0);
    break;

  case Qt::Key_Q:
    vue.rotate(petit_angle, 0.0, 0.0, -1.0);
    break;

  case Qt::Key_E:
    vue.rotate(petit_angle, 0.0, 0.0, +1.0);
    break;

  case Qt::Key_Home:
    vue.initializePosition(*systeme);
    break;

  case Qt::Key_Space:
	pause();
	break;

  case Qt::Key_T:
      vue.dessine_etat(*systeme);
      break;
  };

  update(); // redessine
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
  lastMousePosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  /* If mouse tracking is disabled (the default), the widget only receives
   * mouse move events when at least one mouse button is pressed while the
   * mouse is being moved.
   *
   * Pour activer le "mouse tracking" if faut lancer setMouseTracking(true)
   * par exemple dans le constructeur de cette classe.
   */
 
  if (event->buttons() & Qt::LeftButton) {
	constexpr double petit_angle(.2); // en degrés
 
	// Récupère le mouvement relatif par rapport à la dernière position de la souris
	QPointF d = event->pos() - lastMousePosition;
	lastMousePosition = event->pos();
 
	vue.rotate(petit_angle * d.manhattanLength(), d.y(), d.x(), 0);
 
	update();
  }
}

// ======================================================================
void GLWidget::timerEvent(QTimerEvent* event)
{
  Q_UNUSED(event);

  const double dt = chronometre.elapsed() * Systeme::time_ratio * 1e-3; // chronomètre donne des ms

  chronometre.restart();

  systeme->evolue(dt, vue);
  update();
}

// ======================================================================
void GLWidget::pause()
{
  if (timerId == 0) {
	// dans ce cas le timer ne tourne pas alors on le lance
	timerId = startTimer(20);
	chronometre.restart();
  } else {
	// le timer tourne alors on l'arrête
	killTimer(timerId);
	timerId = 0;
  }
}
