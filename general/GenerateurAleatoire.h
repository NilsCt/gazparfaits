#pragma once

#include <random>

class GenerateurAleatoire {

private:
    std::default_random_engine generateur;
    std::uniform_real_distribution<double> distribution_uniforme;
    std::normal_distribution<double> distribution_gaussienne;

public:
    GenerateurAleatoire(unsigned int graine = std::random_device()() ) : generateur(graine) {}

    double uniforme(double min, double max) {
        return distribution_uniforme(
                generateur,
                std::uniform_real_distribution<double>::param_type{min, max}
                );
    }
    
    double gaussienne(double moyenne, double ecart_type) {
        return distribution_gaussienne(generateur, 
           std::normal_distribution<double>::param_type{moyenne, ecart_type});
  }

};
