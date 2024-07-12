#pragma once

#include <iostream>
#include <fstream>
#include "SupportADessin.h"
#include "Particule.h"
#include "Enceinte.h"
#include "Systeme.h"

// Interface permettant d'écrire dans un fichier les variables d'état du système au cours du temps
class DataWriter : public SupportADessin {

private:
    static constexpr double periode_echantillonage = 0.5 * Systeme::time_ratio; // écart de temps minimum entre 2 états écrits
    static constexpr unsigned int delai = 15; // ignore les n premiers points (pour que le système s'équilibre)

    std::ofstream fichier;
    double derniere_ecriture;
    unsigned int points;

public:
    // constructeurs
    explicit DataWriter(const std::string& path);

    ~DataWriter();
    DataWriter(DataWriter const&) = delete;
    DataWriter& operator=(DataWriter const&) = delete;

    // méthodes
    virtual void dessine(Enceinte const& __attribute__((unused))) override {}
    virtual void dessine(Systeme const& __attribute__((unused))) override {}
    virtual void dessine(Particule const& __attribute__((unused))) override {}
    virtual void dessine(Neon const& __attribute__((unused))) override {}
    virtual void dessine(Argon const& __attribute__((unused))) override {}
    virtual void dessine(Helium const& __attribute__((unused))) override {}
    virtual void dessine(Suivi const& __attribute__((unused))) override {}

    virtual void dessine_rebond(size_t index __attribute__((unused)), unsigned int face __attribute__((unused))) override {}
    virtual void dessine_avant_choc(const Particule& p1 __attribute__((unused)), const Particule& p2 __attribute__((unused)), size_t index __attribute__((unused))) override {}
    virtual void dessine_apres_choc(const Particule& p1 __attribute__((unused)), const Particule& p2 __attribute__((unused)), size_t index __attribute__((unused))) override {}

    virtual void dessine_etat(const Systeme& systeme __attribute__((unused))) const override {}

    virtual void dessine_donnees(const Systeme& systeme);
};
