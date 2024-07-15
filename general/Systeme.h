#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <map>
#include <string>
#include <queue>
#include "Enceinte.h"
#include "Particule.h"
#include "Dessinable.h"
#include "GenerateurAleatoire.h"
#include "Vecteur3D.h"
#include "SupportADessin.h"
#include "Comportement.h"
#include "Transformation.h"


typedef std::pair<double, double> MesurePression; // pression (first) à un instant t (second)
typedef std::deque<MesurePression> CollectionMesuresPression;

// Représente l'entièreté du système physique simulé (enceinte et particules)
class Systeme : public Dessinable {

private:
    std::unique_ptr<Enceinte> enceinte; // polymorphisme, car on a ajouté des Enceintes déformables
    std::vector<std::unique_ptr<Particule>> particules;
    GenerateurAleatoire generateur_aleatoire;
    std::unique_ptr<Comportement> comportement;

    double temps; // temps écoulé depuis le début de la simulation
    Vecteur3D quantite_mouvement_totale;
    double energie_cinetique_totale;
    Vecteur3D centre_masse;
    double masse_totale; // utilisée pour calculer le centre de masse

    CollectionMesuresPression mesures_pression; // pas vraiment la pression, mais plutôt l'impulsion qui permet de déterminer la pression
    std::map<std::string, CollectionMesuresPression> pressions_partielles; // mesures des pressions pour chaque type de particule

    std::queue<std::unique_ptr<Transformation>> liste_transformations;

    static constexpr double Na = 6.022140e23;
    static constexpr double k_b = Particule::R / Na;

    static constexpr unsigned int max_points_pression = 10; // pour faire une moyenne et réduire la variabilité de la pression à un instant t

    double calcule_pression(const CollectionMesuresPression&) const;

    void demarrer_prochaine_transformation();

public:
    static constexpr double time_ratio = 1e-11;

    // constructeurs et opérateurs
    Systeme(double hauteur, double largeur, double profondeur);
    explicit Systeme(std::unique_ptr<Comportement> && comportement_);
    explicit Systeme(std::unique_ptr<Enceinte> && enceinte_ = std::make_unique<Enceinte>(), std::unique_ptr<Comportement> && comportement_ = std::make_unique<ComportementNaif>());
    Systeme(const Systeme& autre) = delete; // pas de copie
    Systeme& operator=(const Systeme&) = delete; // pas d'affectations
    Systeme(Systeme&&) = delete; // pas de déplacement
    Systeme& operator=(Systeme&&) = delete;

    // méthodes
    virtual std::ostream& affiche(std::ostream&) const override;
    void affiche_detaille(std::ostream&) const;
    void affiche_etat_thermo(std::ostream&) const;

    const Enceinte& get_enceinte() const { return *enceinte; } // besoin pour initialiser la vue graphique

    void ajouter_particule(std::unique_ptr<Particule> && p);

    template<typename P>
    void initialiser_particules_precises(unsigned int nb_particules, double temperature);
    void initialiser_particules(unsigned int nb_particules, double temperature);

    void vider();

    virtual void dessine_sur(SupportADessin& support) const override;

    // ajoute l'énergie cinétique d'une particule à l'énergie cinétique totale (pareil pour quantité de mouvement)
    void ajoute_energie(const Particule& particule);
    void supprime_energie(const Particule& particule);
    double energie_cinetique_moyenne() const;
    double pression_theorique() const;
    double pression_effective() const;
    double temperature() const;
    double volume() const { return enceinte->volume(); }
    std::vector<double> get_pressions_partielles() const;

    virtual void evolue(double dt, SupportADessin&) override;

    void ajoute_mesure_pression(double pression);
    void ajoute_mesure_pression_partielle(const std::string& type, double pression);

    double get_temps() const { return temps; }
    void ajoute_transformation(std::unique_ptr<Transformation> && transformation);
    double get_coef_chauffe(double dt) const;

    void initialiser_cycle_carnot(double duree, 
    	double t_a, double v_a, 
    	double t_c, double v_c);
    void initialiser_cycle_stirling(double duree,
        double t_a, double v_a, 
    	double t_c, double v_c);


    // simule l'évolution d'un système (utile pour les tests)
    void simulation(std::ostream& flot);
};

// opérateurs
std::ostream& operator<<(std::ostream&, const Systeme&);
