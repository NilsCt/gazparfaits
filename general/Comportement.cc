#include <iostream>
#include <memory>
#include <cmath>
#include <map>
#include <unordered_set>
#include <tuple>
#include <string>
#include <typeinfo>
#include "Particule.h"
#include "Systeme.h"
#include "SupportADessin.h"
#include "Enceinte.h"
#include "GenerateurAleatoire.h"
#include "Comportement.h"
#include "Vecteur3D.h"

using namespace std;

void Comportement::clear_pressions() {
    tmp_p = 0;
    tmp_part_p.clear();
}

void Comportement::ajoute_pression(Systeme& systeme) {
    systeme.ajoute_mesure_pression(tmp_p);
    for(auto const& e : tmp_part_p) {
        systeme.ajoute_mesure_pression_partielle(e.first, e.second);
    }
}

void Comportement::ajoute_pression_partielle(const Particule& particule, double p) {
    string nom(typeid(particule).name());
    if (tmp_part_p.find(nom) == tmp_part_p.end()) {
        tmp_part_p[nom] = 0;
    }
    tmp_part_p[nom] += p;
}

/*
 * Fais rebondir une particule qui a dépassé une paroi
 * L'origine du repère est placé en bas à gauche de l'enceinte
 * (Voir le schéma dans le rendu "CONCEPTION")
 * En 3D il y a maximum 3 rebonds (1 selon chaque plan)
 * Si la particule va trop vite ou que l'enceinte est trop petite, ce principe est faux.
 * Mais pour optimiser cette méthode qui est appelée un grand nombre de fois (une fois pour chaque particule
 * à chaque évolution, on ne traite pas ce cas)
 */
void Comportement::faire_rebondir(Particule& particule, const Enceinte& enceinte, SupportADessin& support, size_t index) {
    Vecteur3D position(particule.get_position()); // part du principe que la particule ne rebondit pas contre 2 faces opposées
    for(auto coord : Vecteur3D::coords) { // rebond contre les axes x = 0 ; y = 0 ; z = 0
        bool rebondi(false);
        if(position.get_coord(coord) < 0) {
            rebondi = true;
            particule.oppose_position(coord); // inverse la composante de la position et de la vitesse lié au plan de rebond
            particule.oppose_vitesse(coord);
            support.dessine_rebond(index, coord + 1);
        }
        double dim(enceinte.get_dimension(coord));
        if(position.get_coord(coord) > dim) { // rebond contre les axes x = largeur : y = profondeur ; z = hauteur;
            rebondi = true;
            double pos(position.get_coord(coord));
            particule.set_coord_position(coord,  2 * dim - pos); // position symétrique (voir schéma)
            particule.oppose_vitesse(coord); // on inverse la vitesse dans la direction du rebond
            support.dessine_rebond(index, coord + 4);
        }
        if(rebondi) {
            double p(2 * particule.get_masse_reelle() * abs(particule.get_vitesse().get_coord(coord)));
            tmp_p += p; // ajoute la modification de la quantité de mouvement dû au rebond
            ajoute_pression_partielle(particule,p);
        }
    }
}

void Comportement::faire_choc(Systeme& systeme, Particule& p1, Particule& p2, GenerateurAleatoire& generateur_aleatoire) const {
    systeme.supprime_energie(p2); // pas besoin de le faire pour p1 (car pris en compte dans évolue)
    p1.modifier_vitesse_choc(p2, generateur_aleatoire);
    systeme.ajoute_energie(p2);
}

/*
 * Fais évoluer le système (méthode déterministe)
 * NOTE : on a changé la convention des numéros de faces (voir fichier CONCEPTION)
 * On demande autant de paramètres pour éviter les getters dans Systeme (même si c'est moins maniable)
 */
void ComportementNaif::faire_evoluer(
        Systeme& systeme,
        Enceinte& enceinte,
        std::vector<std::unique_ptr<Particule>> & particules,
        GenerateurAleatoire& generateur_aleatoire,
        double dt,
        SupportADessin& support
) {
    clear_pressions();
    enceinte.evolue(dt, support);

    size_t compteur(0);
    for(auto& particule : particules) {
        ++compteur;
        systeme.supprime_energie(*particule);

        particule->evolue(dt, support);
        particule->echange_chaleur(systeme.get_coef_chauffe(dt));
        faire_rebondir(*particule, enceinte, support, compteur);

/* L'énoncé dit que si plusieurs particules rentrent en contact, on choisit une particule aléatoire pour faire le choc
 * pour la méthode déterministe, on considère que choisir la première particule qui est en contact
 * est un choix aléatoire.
 */
        for(auto const& p : particules) {
            if(sont_en_contact(*particule, *p)) {
                support.dessine_avant_choc(*particule, *p, compteur);
                faire_choc(systeme, *particule, *p, generateur_aleatoire);
                support.dessine_apres_choc(*particule, *p, compteur);
                break; // on ne fait qu'un choc
            }
        }

        systeme.ajoute_energie(*particule);
    }

    ajoute_pression(systeme);
}

// Return true si la particule entre en contact avec l'autre particule
bool ComportementNaif::sont_en_contact(const Particule& p1, const Particule& p2) const {
    // pas de chocs avec soi-même
    return p1.get_position().pavage(pas_espace) == p2.get_position().pavage(pas_espace) and &p1 != &p2;
}



void ComportementDynamique::faire_evoluer(
        Systeme& systeme,
        Enceinte& enceinte,
        std::vector<std::unique_ptr<Particule>> & particules,
        GenerateurAleatoire& generateur_aleatoire,
        double dt,
        SupportADessin& support
) {
    clear_pressions();
    enceinte.evolue(dt, support);

    size_t compteur(0);
    for(auto& particule : particules) {
        ++compteur;
        systeme.supprime_energie(*particule);
        supprimer_particule(*particule); // enlève du tableau

        particule->evolue(dt, support);
        particule->echange_chaleur(systeme.get_coef_chauffe(dt));
        faire_rebondir(*particule, enceinte, support, compteur);
        nouvelle_particule(*particule); // remet dans le tableau

        Particule* cible(get_particule_choc(*particule)); // autre particule pour choc
        if(cible != nullptr) {
            support.dessine_avant_choc(*particule, *cible, compteur);
            faire_choc(systeme, *particule, *cible, generateur_aleatoire);
            support.dessine_apres_choc(*particule, *cible, compteur);
        }

        systeme.ajoute_energie(*particule);
    }

    ajoute_pression(systeme);
}

void ComportementDynamique::nouvelle_particule(Particule& particule) {
    Cellule& cellule(get_cellule(particule.get_position()));
    cellule.insert(&particule);
}

void ComportementDynamique::clear_particules() {
    tableau.clear();
}

void ComportementDynamique::supprimer_particule(Particule& particule) {
    Vecteur3D position(particule.get_position());
    Cellule& cellule(get_cellule(position));
    cellule.erase(&particule);
    if(cellule.empty()) {
    	tableau.erase(get_etiquette(position));
    }
}

Particule* ComportementDynamique::get_particule_choc(const Particule& particule) {
    Cellule& cellule(get_cellule(particule.get_position()));
    for(auto p : cellule) { // (pas de référence pour un pointeur à la c)
        if(p != nullptr and &particule != p) return p; // pas de rebond avec soi-même
    }
    return nullptr; // aucune autre particule pour le choc
}

Etiquette ComportementDynamique::get_etiquette(const Vecteur3D& position) {
    unsigned int i(position.get_x() / pas_espace), j(position.get_y() / pas_espace), k(position.get_z() / pas_espace);
    return Etiquette(i,j,k);
}

// get "absolu" : si aucune cellule n'existe pour cette position, la méthode en ajoute une
Cellule& ComportementDynamique::get_cellule(const Vecteur3D& position) {
    Etiquette etiquette(get_etiquette(position));

    // si la cellule n'est pas présente, on l'ajoute
    if (tableau.find(etiquette) == tableau.end()) {
        tableau[etiquette] = Cellule(); // fait une copie et de l'étiquette et de la cellule dans le map
    }
    
    return tableau[etiquette];
}
