TEMPLATE = lib

CONFIG = staticlib
QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH = ../text

SOURCES += \
    Enceinte.cc \
    Particule.cc \
    Systeme.cc \
    Comportement.cc \
    Transformation.cc \
    Vecteur3D.cc \
    \
    ../text/TextViewer.cc
    ../text/FileWriter.cc
    ../text/DataWriter.cc

HEADERS += \
    Dessinable.h \
    Enceinte.h \
    GenerateurAleatoire.h \
    Particule.h \
    SupportADessin.h \
    Systeme.h \
    Comportement.h \
    Transformation.h \
    Vecteur3D.h \
    \
    ../text/TextViewer.h
    ../text/FileWriter.h
    ../text/DataWriter.h
