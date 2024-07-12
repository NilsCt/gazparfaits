#pragma once

#include <string>
#include <array>
#include "GenerateurAleatoire.h"

enum Coord { COORD_X, COORD_Y, COORD_Z };

// Représente un vecteur de dimension 3
class Vecteur3D {
private:
    // les composantes
    double x, y, z;

    constexpr static double precision = 1e-10; // précision par défaut des vecteurs

public:
    static constexpr std::array<Coord, 3> coords = {COORD_X, COORD_Y, COORD_Z};
    
    static constexpr double unite_espace = 1e-10;

    // constructeurs
    explicit Vecteur3D();
    Vecteur3D(double, double, double);

    // Factory
    static Vecteur3D aleatoire_norme_fixe(GenerateurAleatoire&, double); // génère un vecteur de direction aléatoire et norme précisée

    // génère un vecteur aléatoire avec des composantes suivant une loi gaussienne (moy 0, variance précisée)
    static Vecteur3D aleatoire_gaussienne(GenerateurAleatoire&, double variance);

    // génère un vecteur aléatoire avec des composantes suivant une loi uniforme (min 0, max précisé)
    static Vecteur3D aleatoire_uniforme(GenerateurAleatoire&, double max_x, double max_y, double max_z);

    // interface
    double get_x() const;
    double get_y() const;
    double get_z() const;
    double get_coord(Coord) const;

    void set_coord(Coord, double);
    Vecteur3D& set_all_coord(double, double, double); // utilisé pour les opérateurs

    // méthodes
    std::ostream& affiche(std::ostream&) const;
    double norme2() const;
    double norme() const;
    double angle(const Vecteur3D&) const;
    void coord_oppose(Coord);
    Vecteur3D pavage(double epsilon) const;

    // opérateurs internes
    bool operator==(const Vecteur3D&) const;
    bool operator!=(const Vecteur3D&) const;

    Vecteur3D& operator+=(const Vecteur3D&);
    Vecteur3D& operator*=(double);
    Vecteur3D& operator/=(double);
    Vecteur3D& operator-=(const Vecteur3D&);
    Vecteur3D& operator^=(const Vecteur3D&); // produit vectoriel
    double operator*(const Vecteur3D&) const; // produit scalaire
    const Vecteur3D operator~() const; // vecteur unitaire
};

// opérateurs externes
std::ostream& operator<<(std::ostream&, const Vecteur3D&);
const Vecteur3D operator+(Vecteur3D, const Vecteur3D&);
const Vecteur3D operator*(Vecteur3D, double); // multiplication par un scalaire
const Vecteur3D operator*(double, const Vecteur3D&);
const Vecteur3D operator/(Vecteur3D, double);
const Vecteur3D operator-(Vecteur3D); // opposé
const Vecteur3D operator-(Vecteur3D, const Vecteur3D&);
const Vecteur3D operator^(Vecteur3D, const Vecteur3D&); // produit vectoriel

