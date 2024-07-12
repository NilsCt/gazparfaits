#pragma once

#include "Enceinte.h"

// Stocke les données relatives à une transformation du système
class Transformation {

protected:
    double duree;
    double debut;

public:
    Transformation(double duree_);
    
    virtual ~Transformation() = default;
    Transformation(const Transformation&) = default; // on rétablit les copies
    Transformation& operator=(const Transformation&) = default;
    Transformation(Transformation&&) = default;
    Transformation& operator=(Transformation&&) = default;

    virtual void demarrer(double debut_, double t_i __attribute__((unused)), double v_i __attribute__((unused)), double time_ratio __attribute__((unused)));

    // le coef de chauffe dépend du temps pour une adiabatique
    virtual double get_coef_chauffe(double t, double dt, double gamma) const = 0;

    virtual Enceinte* construire_enceinte(double l_i, double p_i, double h_i) const = 0;

    bool est_terminee(double t, double ratio) const;
};


class Pause : public Transformation {
public:
    using Transformation::Transformation;
    
    virtual double get_coef_chauffe(double t __attribute__((unused)), double dt __attribute__((unused)), double gamma __attribute__((unused))) const { return 1; }
    
    virtual Enceinte* construire_enceinte(double l_i __attribute__((unused)), double p_i __attribute__((unused)), double h_i __attribute__((unused))) const { return nullptr; }
};

class ModificationTemperature : public Transformation {
private:
    double temperature_finale;
    double coef_chauffe;

public:
    ModificationTemperature(double t_f, double duree_);

    // Initialise le coef de chauffe
    // On ne le fait pas lors du constructeur, car les transformations peuvent être
    // mises en attente (il faut prendre les valeurs du début de transformation)
    virtual void demarrer(double debut_, double t_i, double v_i, double time_ratio) override;

    virtual double get_coef_chauffe(double t __attribute__((unused)), double dt, double gamma __attribute__((unused))) const override;

    virtual Enceinte* construire_enceinte(double l_i __attribute__((unused)), double p_i __attribute__((unused)), double h_i __attribute__((unused))) const override { return nullptr; }
};

class ModificationEnceinte : public Transformation {
protected:
    double largeur_finale;
    double profondeur_finale;
    double hauteur_finale;

public:
    ModificationEnceinte(double l_f, double p_f, double h_f, double dur);

    virtual ~ModificationEnceinte() = default;
    ModificationEnceinte(const ModificationEnceinte&) = default; // on rétablit les copies
    ModificationEnceinte& operator=(const ModificationEnceinte&) = default;
    ModificationEnceinte(ModificationEnceinte&&) = default;
    ModificationEnceinte& operator=(ModificationEnceinte&&) = default;

    virtual Enceinte* construire_enceinte(double l_i, double p_i, double h_i) const override;
};

class TravailIsotherme : public ModificationEnceinte {
public:
    using ModificationEnceinte::ModificationEnceinte;

    virtual double get_coef_chauffe(double t __attribute__((unused)), double dt __attribute__((unused)), double gamma __attribute__((unused))) const override { return 1; }
};

class TravailAdiabatique : public ModificationEnceinte {
private:
    double volume_initial;
    double coef_compression;

public:
    TravailAdiabatique(double l_f, double p_f, double h_f, double dur);

    virtual void demarrer(double debut_, double t_i, double v_i, double time_ratio) override;

    virtual double get_coef_chauffe(double t, double dt, double gamma) const override;
};
