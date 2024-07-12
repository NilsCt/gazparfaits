QMAKE_CXXFLAGS += -std=c++17

CONFIG += console

TARGET = simulation

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

INCLUDEPATH = ../general

SOURCES += \
    #testVecteur.cc \
    #testParticule.cc \
    #testEnceinte.cc \
    #exerciceP7.cc \
    #testSysteme1.cc \
    #exerciceP9.cc \
    #testEvolution.cc \
    #testFileWriter.cc \
    #testCompression.cc \
    #testSuivi.cc \
    testDataWriter.cc \
    \
    FileWriter.cc \
    TextViewer.cc \
    DataWriter.cc \
    \
    ../general/Enceinte.cc \
    ../general/Particule.cc \
    ../general/Systeme.cc \
    ../general/Comportement.cc \
    ../general/Transformation.cc \
    ../general/Vecteur3D.cc

HEADERS += \
    FileWriter.h \
    TextViewer.h \
    DataWriter.h \
    \
    ../general/Dessinable.h \
    ../general/Enceinte.h \
    ../general/GenerateurAleatoire.h \
    ../general/Particule.h \
    ../general/SupportADessin.h \
    ../general/Systeme.h \
    ../general/Comportement.h \
    ../general/Transformation.h \
    ../general/Vecteur3D.h
