#pragma once

#include "Particule.h"
#include "Systeme.h"
#include "SupportADessin.h"
#include "Enceinte.h"
#include "GenerateurAleatoire.h"
#include "Vecteur3D.h"
#include <tuple>
#include <vector>
#include <map>
#include <unordered_set>
#include <string>

// Représente le protocole qui détermine quelles particules se rencontrent et comment modifier les trajectoires
class Comportement {

protected:
    double tmp_p; // variables temporaires utilisées pour calculer dynamiquement les pressions
    std::map<std::string, double> tmp_part_p;

    void clear_pressions();
    void ajoute_pression(Systeme& systeme);

private:
    void ajoute_pression_partielle(const Particule& particule, double p);

public:
    virtual ~Comportement() = default; // supprime implicitement les constructeurs par défaut
    Comportement() = default;
    // on ne rétablit pas les constructeurs de copies du comportement

    // On demande autant de paramètres pour éviter les getters dans Systeme (même si c'est moins maniable)
    virtual void faire_evoluer(
            Systeme& systeme,
            Enceinte& enceinte, // pour ne pas faire des getters dans système qui casserait l'encapsulation
            std::vector<std::unique_ptr<Particule>> & particules,
            GenerateurAleatoire& generateur_aleatoire,
            double dt,
            SupportADessin& support
    ) = 0;

    // Pas virtual car commun à tous les protocoles
    void faire_rebondir(Particule& particule, const Enceinte& enceinte, SupportADessin& support, size_t index);

    // Modifie les trajectoires de 2 particules qui s'entrechoquent
    // Pas virtual car commun à tous les protocoles
    void faire_choc(Systeme& systeme, Particule& p1, Particule& p2, GenerateurAleatoire& generateur_aleatoire) const;

    virtual void nouvelle_particule(Particule& particule) = 0;
    virtual void clear_particules() = 0;
};

// Premier protocole réalisé
class ComportementNaif : public Comportement {
private:
    static constexpr double pas_espace = 1;
public:

    virtual void faire_evoluer(
            Systeme& systeme,
            Enceinte& enceinte,
            std::vector<std::unique_ptr<Particule>> & particules,
            GenerateurAleatoire& generateur_aleatoire,
            double dt,
            SupportADessin& support
    ) override;

    // Return true si la particule entre en contact avec l'autre particule
    bool sont_en_contact(const Particule& p1, const Particule& p2) const;

    virtual void nouvelle_particule(Particule& particule __attribute__((unused))) override {};
    virtual void clear_particules() override {};
};

typedef std::unordered_set<Particule*> Cellule; // pointeurs simples, car déjà des uniques_ptr dans systeme
typedef std::tuple<unsigned int, unsigned int, unsigned int> Etiquette;
typedef std::map<Etiquette, Cellule> Tableau;

// Second protocole réalisé (P14)
class ComportementDynamique : public Comportement {
private:
    static constexpr double pas_espace = 0.5;

    Tableau tableau;

    Etiquette get_etiquette(const Vecteur3D& position); // donne la clé pour une cellule dans le map
    Cellule& get_cellule(const Vecteur3D& position);
    // PAS CONST CAR PEUT AJOUTER DES NOUVELLES CELLULES (DETENTE ENCEINTE)

    /*
     * Cherche dans la même case que la particule une autre particule pour faire un choc
     * Renvoie nullptr s'il n'y a aucune autre particule pour un choc
     */
    Particule* get_particule_choc(const Particule& particule);

public:

    virtual void faire_evoluer(
            Systeme& systeme,
            Enceinte& enceinte,
            std::vector<std::unique_ptr<Particule>> & particules,
            GenerateurAleatoire& generateur_aleatoire,
            double dt,
            SupportADessin& support
    ) override;

    void nouvelle_particule(Particule& particule) override;
    void clear_particules() override;
    void supprimer_particule(Particule& particule);
};
