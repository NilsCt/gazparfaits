#pragma once


// pour empêcher les dépendances circulaires
class Enceinte;
class Systeme;
class Neon;
class Argon;
class Helium;
class Particule;
class Suivi;

// Représente une interface où l'on peut afficher l'état d'un système
class SupportADessin {
public:
    virtual ~SupportADessin() = default;
    SupportADessin(SupportADessin const&) = delete;
    SupportADessin& operator=(SupportADessin const&) = delete;
    // en réalité le destructeur virtuel supprime déjà les constructeurs de copies
    SupportADessin(SupportADessin&&) = default; // mais on veut pouvoir les déplacer
    SupportADessin& operator=(SupportADessin&&) = default;

    SupportADessin() = default; // obligatoire (car supprimé par le destructeur virtuel)

    // Pour dessiner des objets héritant de Dessinable
    virtual void dessine(Enceinte const&) = 0; // on est obligé de tout mettre même si lien d'héritage (énoncé)
    virtual void dessine(Systeme const&) = 0;
    virtual void dessine(Particule const&) = 0;
    virtual void dessine(Neon const&) = 0;
    virtual void dessine(Argon const&) = 0;
    virtual void dessine(Helium const&) = 0;
    virtual void dessine(Suivi const&) = 0;

    virtual void dessine_rebond(size_t index __attribute__((unused)), unsigned int face __attribute__((unused))) {}
    virtual void dessine_avant_choc(const Particule& p1 __attribute__((unused)), const Particule& p2 __attribute__((unused)), size_t index __attribute__((unused))) {}
    virtual void dessine_apres_choc(const Particule& p1 __attribute__((unused)), const Particule& p2 __attribute__((unused)), size_t index __attribute__((unused))) {}

    // dessine les grandeurs d'état du système (pression, température, ...) quand on presse un bouton
    virtual void dessine_etat(const Systeme& systeme) const = 0;
    // écrit toutes les données pour un instant t (exemple dans un fichier)
    virtual void dessine_donnees(const Systeme& systeme __attribute__((unused))) {};
    
    virtual void replacer_camera(const Systeme& __attribute__((unused))) {}
};
