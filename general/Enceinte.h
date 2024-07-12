#pragma once

#include <string>
#include <cmath>
#include "Dessinable.h"
#include "Vecteur3D.h"
#include "SupportADessin.h"

// Représente l'enceinte enfermant les molécules (un pavé droit)
class Enceinte : public Dessinable {

protected:
	double hauteur; // pas const pour la sous classe EnceinteDeformable
	double largeur;
	double profondeur;
	
public:
    // constructeurs
	Enceinte(double h, double l, double p); // attention pas l'ordre x y z, mais l'ordre de l'énoncé (voir fichier CONCEPTION)
	Enceinte();
	
    virtual ~Enceinte() = default;
    Enceinte(const Enceinte&) = default; // on rétablit les copies
    Enceinte& operator=(const Enceinte&) = default;
    Enceinte(Enceinte&&) = default;
    Enceinte& operator=(Enceinte&&) = default;

    // méthodes
    double get_dimension(Coord) const;
    double get_largeur() const { return largeur; }
    double get_profondeur() const { return profondeur; }
    double get_hauteur() const { return hauteur; }
    double volume() const;
    double surface() const;
    Vecteur3D centre() const;
    virtual double get_largeur_max() const { return largeur; }
    virtual double get_profondeur_max() const { return profondeur; }
    virtual double get_hauteur_max() const { return hauteur; }
    

	virtual std::ostream& affiche(std::ostream&) const override;

    virtual void dessine_sur(SupportADessin& support) const override;

    virtual void evolue(double dt __attribute__((unused)), SupportADessin& __attribute__((unused))) override {};
};

// opérateurs
std::ostream& operator<<(std::ostream&, const Enceinte&);

// Représente une enceinte déformable (sa taille évolue dans le temps)
class EnceinteDeformable : public Enceinte {
private:
    const double hauteur_finale; // déformation de l'enceinte
    const double largeur_finale;
    const double profondeur_finale;
    
    const double vx;
    const double vy;
    const double vz;
    
    static bool compare(double x, double y);

public:
    EnceinteDeformable(double h_i, double l_i, double p_i, double h_f, double l_f, double p_f, double d);
    
    virtual void evolue(double dt, SupportADessin& __attribute__((unused))) override;
    
    virtual double get_largeur_max() const override { return std::max(largeur, largeur_finale); }
    virtual double get_profondeur_max() const override { return std::max(profondeur, profondeur_finale); }
    virtual double get_hauteur_max() const override { return std::max(hauteur, hauteur_finale); }
};

