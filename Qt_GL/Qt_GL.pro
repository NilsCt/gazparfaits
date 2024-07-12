QT += core gui opengl widgets
contains(QT_VERSION, ^6\\..*) {
    QT += openglwidgets
}
QMAKE_CXXFLAGS += -std=c++17

win32:LIBS += -lopengl32


TARGET = simulation

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH = \
    ../general \
    ../text

SOURCES += \
    #testGraphique.cc \ 
    #testCompression.cc \
    #testDetente.cc \ 
    #exerciceP12.cc \  
    #testSuivi.cc \
    #testComportement.cc \
    #testDetenteDynamique.cc \
    testEtat.cc \
    #testTransformations.cc \
    #testPerformances.cc \
    \
    glwidget.cc \
    vue_opengl.cc \
    glsphere.cc \
    \
    ../general/Enceinte.cc \
    ../general/Particule.cc \
    ../general/Systeme.cc \
    ../general/Comportement.cc \
    ../general/Transformation.cc \
    ../general/Vecteur3D.cc \
    \
    ../text/TextViewer.cc
    ../text/FileWriter.cc
    ../text/DataWriter.cc

HEADERS += \
    glwidget.h \
    vertex_shader.h \
    vue_opengl.h \
    glsphere.h \
    \
    ../general/Dessinable.h \
    ../general/Enceinte.h \
    ../general/GenerateurAleatoire.h \
    ../general/Particule.h \
    ../general/SupportADessin.h \
    ../general/Systeme.h \
    ../general/Comportement.h \
    ../general/Transformation.h \
    ../general/Vecteur3D.h \
    \
    ../text/TextViewer.h
    ../text/FileWriter.h
    ../text/DataWriter.h

RESOURCES += \
    resource.qrc
