#pragma once

class SupportADessin;

// Représente un objet qui peut être "dessiné", c'est-à-dire dont l'état peut être représenté dans un SupportADessin
class Dessinable {

public:
    /*
     * Dessine l'objet sur le support fourni
     * Énoncé : il ne faut pas définir cette méthode ici, mais pour chaque sous classe
     */
    virtual void dessine_sur(SupportADessin&) const = 0;

    virtual ~Dessinable() = default;
    Dessinable(const Dessinable&) = default; // les constructeurs sont supprimés par le destructeur virtuel
    Dessinable& operator=(const Dessinable&) = default;
    Dessinable(Dessinable&&) = default;
    Dessinable& operator=(Dessinable&&) = default;

    Dessinable() = default;

    virtual void evolue(double dt, SupportADessin&) = 0;

    virtual std::ostream& affiche(std::ostream&) const = 0;
};