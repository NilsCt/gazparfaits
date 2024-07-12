#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include "Systeme.h"
#include "Enceinte.h"
#include "GenerateurAleatoire.h"
#include "Vecteur3D.h"
#include "TextViewer.h"
#include "SupportADessin.h"
#include "Comportement.h"
using namespace std;


// Constructeurs
Systeme::Systeme(
        std::unique_ptr<Enceinte> && enceinte_,
        std::unique_ptr<Comportement> && comportement_
) : enceinte(std::move(enceinte_)), // std sinon warning
    comportement(std::move(comportement_)),
    temps(0), energie_cinetique_totale(0), masse_totale(0) {}

Systeme::Systeme(double h, double l, double p) : Systeme(make_unique<Enceinte>(h,l,p)) {}

Systeme::Systeme(std::unique_ptr<Comportement> && comportement_) :
    Systeme(make_unique<Enceinte>(), std::move(comportement_)) {}

// Méthodes
void Systeme::affiche_detaille(ostream& sortie) const {
    unsigned int size(particules.size());
    if(size == 0) {
        sortie << "Le système ne contient aucune particule" << endl;
    } else {
        sortie << "Le système est constitué de " << particules.size() << " particules :" << endl;
        for(auto const& particule : particules) {
            sortie << *particule;
        }
    }
    sortie << "Dans une enceinte :" << endl << *enceinte;
}

// Affiche juste les particules une à une
ostream& Systeme::affiche(ostream& sortie) const {
    if(particules.empty()) {
        sortie << "Le système ne contient aucune particule" << endl;
    } else {
        for(auto const& particule : particules) {
            sortie << *particule;
        }
    }
    return sortie;
}

// Affiche les grandeurs thermodynamiques
void Systeme::affiche_etat_thermo(std::ostream& sortie) const {
    sortie << "====================" << endl;
    sortie << "Température : " << temperature() << " K" << endl;
    sortie << "Énergie interne : " << energie_cinetique_totale << " J" << endl;
    sortie << "Volume : " << enceinte->volume() << " m3" << endl;
    sortie << "Masse : " << masse_totale << " kg" << endl;
    sortie << "Temps écoulé : " << temps << " s" << endl;
    sortie << endl << "Loi des gaz parfaits :" << endl;
    sortie << "    Pression théorique : " << pression_theorique() << " Pa"<< endl;
    sortie << "    Pression effective : " << pression_effective() << " Pa"<< endl;
    sortie << endl << "Loi de Dalton :" << endl;
    double s(0);
    for(auto const& e : pressions_partielles) {
        double p(calcule_pression(e.second));
        s += p;
        sortie << "    pression partielle de " << e.first << " : " << p << " Pa" << endl;
    }
    sortie << "Somme des pressions partielles : " << s << " Pa" << endl;
    sortie << endl << "Mouvement Brownien :" << endl;
    sortie << "    Quantité de mouvement théorique : " << Vecteur3D() << endl;
    sortie << "    Quantité de mouvement : " << quantite_mouvement_totale << endl;
    sortie << "    Centre de masse théorique : " << enceinte->centre() << endl;
    sortie << "    Centre de masse : " << centre_masse << endl;
    sortie << "====================" << endl;
}

// Ajoute une particule au système
void Systeme::ajouter_particule(unique_ptr<Particule> && p) {
    if(p != nullptr) {
        particules.push_back(std::move(p)); // std sinon warning
        comportement->nouvelle_particule(*particules.back());
        ajoute_energie(*particules.back());
    }
}

/*
 * Initialise un certain nombre de particules spécifiées avec une température donnée
 * Le type de particule est donné en paramètre de template :
 * initialiser_particules_precises<Argon>(...)
 */
template<typename P>
void Systeme::initialiser_particules_precises(unsigned int nb_particules, double temperature) {
    for(unsigned int i(0); i < nb_particules; ++i) {
        Vecteur3D vitesse(Vecteur3D::aleatoire_gaussienne(generateur_aleatoire, temperature * P::constante_specifique));
        Vecteur3D position(Vecteur3D::aleatoire_uniforme(generateur_aleatoire, enceinte->get_largeur(), enceinte->get_profondeur(), enceinte->get_hauteur()));
        ajouter_particule(make_unique<P>(position, vitesse));
    }
}

// Initialise un certain nombre de particules de tous les types avec une température donnée
void Systeme::initialiser_particules(unsigned int nb_particules, double temperature) {
    initialiser_particules_precises<Neon>(nb_particules/3, temperature);
    initialiser_particules_precises<Argon>(nb_particules/3, temperature);
    initialiser_particules_precises<Helium>(nb_particules/3 + nb_particules % 3, temperature); // pour avoir exactement N particules
}

// Supprime toutes les particules du système
void Systeme::vider() {
    // Les unique_ptr se désallouent automatiquement
    particules.clear();
    comportement->clear_particules();
    energie_cinetique_totale = 0;
    quantite_mouvement_totale = Vecteur3D(); // vecteur nul
}

void Systeme::dessine_sur(SupportADessin& support) const {
    for(auto const& particule : particules) {
        particule->dessine_sur(support);
    }
    enceinte->dessine_sur(support);
    support.dessine_donnees(*this);
    // on est obligé de faire une nouvelle méthode dessine_donnees et pas dessine(système) pour respecter l'affichage textuel de l'énoncé
}


void Systeme::ajoute_energie(const Particule &particule) {
    energie_cinetique_totale += particule.energie_cinetique();
    quantite_mouvement_totale += particule.quantite_de_mouvement();
    
    double m(particule.get_masse_reelle());
    centre_masse = (masse_totale * centre_masse + m * particule.get_position()) / (masse_totale + m);
    masse_totale += m;
}

void Systeme::supprime_energie(const Particule &particule) {
    energie_cinetique_totale -= particule.energie_cinetique();
    quantite_mouvement_totale -= particule.quantite_de_mouvement();
    
    double m(particule.get_masse_reelle());
    centre_masse = ((masse_totale + m) * centre_masse - m * particule.get_position()) / masse_totale;
    masse_totale -= m;
}


double Systeme::energie_cinetique_moyenne() const {
    return energie_cinetique_totale / particules.size();
}

// Calcule la température effective dans l'enceinte
double Systeme::temperature() const {
    return 2.0/3 / k_b * energie_cinetique_moyenne();
}

// Calcule la pression théorique dans l'enceinte (en utilisant la température effective)
double Systeme::pression_theorique() const {
    // return 2.0/3 * particules.size() * energie_cinetique_moyenne() / enceinte->volume();
    return particules.size() * k_b * temperature() / enceinte->volume();   
}

double Systeme::calcule_pression(const CollectionMesuresPression & mesures) const {
    if(mesures.empty()) return 0;
    //double delta_t(mesures.back().second - mesures.front().second);
    double delta_t(temps - mesures.front().second);
    double delta_p(0);
    for(auto const& mes : mesures) {
        delta_p += mes.first;
    }
    return delta_p / delta_t / enceinte->surface();
}

// Calcule la pression effective dans l'enceinte (force excercée par les rebonds sur la paroi)
double Systeme::pression_effective() const {
    return calcule_pression(mesures_pression);
}

vector<double> Systeme::get_pressions_partielles() const {
    vector<double> liste;
    for(auto const& e : pressions_partielles) {
        double p(calcule_pression(e.second));
        liste.push_back(p);
    }
    return liste;
}

/*
 * Fais évoluer le système (méthode déterministe)
 * NOTE : on a changé la convention des numéros de faces (voir fichier CONCEPTION)
 */
void Systeme::evolue(double dt, SupportADessin& support) {
    temps += dt;
    comportement->faire_evoluer(*this,*enceinte, particules, generateur_aleatoire, dt, support);

    if(not liste_transformations.empty() and liste_transformations.front()->est_terminee(temps, time_ratio)) { // si la transformation est finie
        liste_transformations.pop();
        demarrer_prochaine_transformation(); // s'il y a encore une transformation
    }
}

void Systeme::ajoute_mesure_pression(double pression) {
    mesures_pression.push_back(MesurePression(pression, temps));
    if(mesures_pression.size() > max_points_pression) mesures_pression.pop_front();
}

// Ajoute une mesure d'une pression partielle pour un type de particule
void Systeme::ajoute_mesure_pression_partielle(const string& type, double pression) {
    if (pressions_partielles.find(type) == pressions_partielles.end()) { // si pas dans la liste des pressions partielles
        pressions_partielles[type] = CollectionMesuresPression();
    }
    CollectionMesuresPression& col(pressions_partielles[type]);
    col.push_back(MesurePression(pression, temps));
    if(col.size() > max_points_pression) col.pop_front();
}


void Systeme::demarrer_prochaine_transformation() {
    if(not liste_transformations.empty()) {
        Transformation& transformation(*liste_transformations.front());
        transformation.demarrer(temps, temperature(), enceinte->volume(), time_ratio);
        Enceinte* enc(transformation.construire_enceinte(
        	enceinte->get_largeur(),
        	enceinte->get_profondeur(),
        	enceinte->get_hauteur()
        	));
        if(enc != nullptr) {
        	enceinte = unique_ptr<Enceinte>(enc);
        }
    }
}

void Systeme::ajoute_transformation(unique_ptr<Transformation>&& transformation) {
    liste_transformations.push(std::move(transformation));
    if(liste_transformations.size() == 1) { // s'il n'y avait pas de transformation avant, il faut la commencer
        demarrer_prochaine_transformation();
    }
}

double Systeme::get_coef_chauffe(double dt) const {
    if(liste_transformations.empty()) return 1;
    else return liste_transformations.front()->get_coef_chauffe(temps, dt, Particule::gamma);
}

// durée pour chaque étape, température et taille de l'enceinte pour les 2 points opposés du cycle
// l'ordre détermine si le cycle est moteur ou frigo
void Systeme::initialiser_cycle_carnot(
	double duree, 
    	double t_a, double v_a, 
    	double t_c, double v_c) {
    double c_a(pow(v_a, 1.0/3)), c_c(pow(v_c, 1.0/3)); // taille côtés de l'enceinte
    ajoute_transformation(make_unique<TravailIsotherme>(c_a,c_a,c_a, 1)); // être sûr d'etre au point de départ
    ajoute_transformation(make_unique<ModificationTemperature>(t_a, 1));	
    
    double v_b(v_c * pow(t_c/t_a, 1/(Particule::gamma - 1)));
    double c_b(pow(v_b, 1.0/3));
    double v_d(v_a * v_c / v_b);
    double c_d(pow(v_d, 1.0/3));
    for(int i(0); i < 100; ++i) {
    	ajoute_transformation(make_unique<TravailIsotherme>(c_b, c_b, c_b, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    	ajoute_transformation(make_unique<TravailAdiabatique>(c_c, c_c, c_c, duree));
    	//ajoute_transformation(make_unique<ModificationTemperature>(t_c, 0.5));
    	// les adiabatiques sont peu précises, cette "fausse" transformation empêche des dérives petit à petit au cours des cycles
    	ajoute_transformation(make_unique<Pause>(1.5));
    	ajoute_transformation(make_unique<TravailIsotherme>(c_d, c_d, c_d, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    	ajoute_transformation(make_unique<TravailAdiabatique>(c_a, c_a, c_a, duree));
    	//ajoute_transformation(make_unique<ModificationTemperature>(t_a, 0.5));
    	ajoute_transformation(make_unique<Pause>(1.5));
    }
}

void Systeme::initialiser_cycle_stirling(
	double duree, 
    	double t_a, double v_a, 
    	double t_c, double v_c) {
    double c_a(pow(v_a, 1.0/3)), c_c(pow(v_c, 1.0/3));
    ajoute_transformation(make_unique<TravailIsotherme>(c_a,c_a,c_a, 1));
    ajoute_transformation(make_unique<ModificationTemperature>(t_a, 1));	
    
    for(int i(0); i < 100; ++i) {
    	ajoute_transformation(make_unique<TravailIsotherme>(c_c, c_c, c_c, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    	ajoute_transformation(make_unique<ModificationTemperature>(t_c, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    	ajoute_transformation(make_unique<TravailIsotherme>(c_a, c_a, c_a, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    	ajoute_transformation(make_unique<ModificationTemperature>(t_a, duree));
    	ajoute_transformation(make_unique<Pause>(2));
    }
}


// simule l'évolution d'un système dans un flot (utile pour les tests)
// (pour éviter le copier-coller)
void Systeme::simulation(ostream& flot) {
    TextViewer textViewer(flot);
    affiche_detaille(flot);
    textViewer.dessine_etat(*this);

    flot << "Lancement de la simulation" << endl;
    for(int i(0); i < 5; ++i) {
        flot << "====================" << endl;
        evolue(10*time_ratio, textViewer);
        dessine_sur(textViewer);
    }
    flot << "====================" << endl;
    /*
     * On regarde si la quantité de mouvement et l'énergie cinétique sont conservés
     * À noter que l'énergie cinétique est toujours conservée, la quantité de mouvement est conservée pour les chocs,
     * MAIS la quantité de mouvement du système totale n'est pas conservé lors d'un rebond d'une particule :
     * en effet la norme et les valeurs absolues des composantes de la quantité de mouvement de la particule étudiée sont conservées,
     * mais dès lors que l'on rajoute d'autres particules dans le système ce n'est plus le cas.
     * Exemple : m (x y z) + (a b c) = (mx+a my+b mz+c) -> m (x y -z) + (a b c) = (mx+a my+b -mz+c)
     * avec (x y z) vitesse de particule qui rebondit et (a b c) quantité de mouvement de particule immobile
     */
    textViewer.dessine_etat(*this);
    flot << endl << endl;
}

// Opérateurs
ostream& operator<<(ostream& sortie, const Systeme& systeme) {
    return systeme.affiche(sortie);
}
