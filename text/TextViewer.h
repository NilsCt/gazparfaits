#pragma once

#include <iostream>
#include "SupportADessin.h"
#include "Particule.h"
#include "Enceinte.h"
#include "Systeme.h"

// Interface permettant d'afficher l'état du système directement dans un iostream
class TextViewer : public SupportADessin {

protected:
    std::ostream& sortie;

public:
    // constructeurs
    explicit TextViewer(std::ostream&);

    virtual ~TextViewer() = default;
    TextViewer(TextViewer const&) = delete; // pas de copie (en réalité supprimé par le destructeur virtuel)
    TextViewer& operator=(TextViewer const&) = delete;
    TextViewer(TextViewer&&) = default; // mais on peut les déplacer
    TextViewer& operator=(TextViewer&&) = default;

    // méthodes
    virtual void dessine(Enceinte const&) override; // l'énoncé nous force à définir toutes ces méthodes
    virtual void dessine(Systeme const&) override;
    virtual void dessine(Particule const&) override;
    virtual void dessine(Neon const&) override;
    virtual void dessine(Argon const&) override;
    virtual void dessine(Helium const&) override;
    virtual void dessine(Suivi const&) override {}

    virtual void dessine_rebond(size_t index, unsigned int face) override;
    virtual void dessine_avant_choc(const Particule& p1, const Particule& p2, size_t index) override;
    virtual void dessine_apres_choc(const Particule& p1, const Particule& p2, size_t index) override;

    virtual void dessine_etat(const Systeme& systeme) const override;
};