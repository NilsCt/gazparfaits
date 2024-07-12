#pragma once
#include "Vecteur3D.h"
#include "Dessinable.h"
#include "GenerateurAleatoire.h"
#include "SupportADessin.h"
#include <deque>
#include <string>
#include <memory>

// Historique des positions d'une particule
class Suivi : public Dessinable {
private:
    Particule& particule;
    std::deque<Vecteur3D> trajectoire;

    static constexpr unsigned int max_points = 70; // nombre maximum de points dans la trajectoire
    static constexpr double distance_min = 0.5; // distance minimum entre 2 points sauvegardés (arrondis les rebonds et chocs mais permet de ne pas stocker trop de données)

public:
    Suivi(Particule& particule);

    virtual std::ostream& affiche(std::ostream& sortie) const override { return sortie; };
    virtual void evolue(double dt __attribute__((unused)), SupportADessin&) override;
    virtual void dessine_sur(SupportADessin& support) const override;

    const std::deque<Vecteur3D>& get_trajectoire() const { return trajectoire; }
};

// Représente une particule
class Particule : public Dessinable {
public:
    static constexpr double R = 8.314472;
    static constexpr double gamma = 5.0/3; // pour particules monoatomiques

protected:
	static constexpr double unite_masse = 1.66e-27; 

	Vecteur3D position;
	Vecteur3D vitesse;
	const double masse;
    std::unique_ptr<Suivi> suivi; // historique des positions (nullptr si pas activé)
    
    // Repère les particules dont on suit l'avancement par une * avant de les afficher
    void marqueur_suivi(std::ostream& sortie) const; 

    // static unsigned int get_face(Coord coord, bool loin); // (plus utilisé)
		
public:
	// constructeurs
	Particule(Vecteur3D const& position, Vecteur3D const& vitesse, double masse, bool est_suivi = false);

    virtual ~Particule() = default;
    Particule(const Particule&); // on rétablit les copies
    Particule& operator=(const Particule&);
    Particule(Particule&&);
    Particule& operator=(Particule&&);

    // méthodes
	virtual std::ostream& affiche(std::ostream&) const override;
    void affiche_court(std::ostream&) const;

    virtual void dessine_sur(SupportADessin& support) const override;

    Vecteur3D quantite_de_mouvement() const;
    double energie_cinetique() const;

    virtual void evolue(double dt, SupportADessin& __attribute__((unused))) override;
    void modifier_vitesse_choc(Particule&, GenerateurAleatoire&);
    void echange_chaleur(double coef);

    // méthodes utiles pour que le comportement puisse agir sur les particules
    Vecteur3D get_position() const { return position; }
    Vecteur3D get_vitesse() const { return vitesse; }
    double get_masse() const { return masse; }
    double get_masse_reelle() const { return masse * unite_masse; }
    void oppose_position(Coord coord) { position.coord_oppose(coord); } // x y z -> x -y z (pour Coord_Y)
    void set_coord_position(Coord coord, double x) { position.set_coord(coord, x); }
    void oppose_vitesse(Coord coord) { vitesse.coord_oppose(coord); }
};

// opérateurs
std::ostream& operator<<(std::ostream&, const Particule&);

// Sous classes : types de particule
class Argon : public Particule {
protected:
    static constexpr double masse_argon = 39.948;
public:
    static constexpr double constante_specifique = R * 1000 / masse_argon;

    Argon(Vecteur3D const&, Vecteur3D const&, bool est_suivi = false);
    virtual std::ostream& affiche(std::ostream&) const override;
    virtual void dessine_sur(SupportADessin& support) const override;
};
// pas besoin de surcharger l'opérateur << car on l'a déjà fait pour Particule (et grâce au polymorphisme)


class Neon : public Particule {
protected:
    static constexpr double masse_neon = 20.1797;
public:
    static constexpr double constante_specifique = R * 1000 / masse_neon;

    Neon(Vecteur3D const&, Vecteur3D const&, bool est_suivi = false);
    virtual std::ostream& affiche(std::ostream&) const override;
    virtual void dessine_sur(SupportADessin& support) const override;
};


class Helium : public Particule {
protected:
    static constexpr double masse_helium = 4.002602;
public:
    static constexpr double constante_specifique = R * 1000 / masse_helium;

    Helium(Vecteur3D const&, Vecteur3D const&, bool est_suivi = false);
    virtual std::ostream& affiche(std::ostream&) const override;
    virtual void dessine_sur(SupportADessin& support) const override;
};
