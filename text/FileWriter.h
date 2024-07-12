#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "TextViewer.h"

// Interface permettant d'afficher l'état du système dans un fichier
class FileWriter : public TextViewer {

private:
    std::ofstream fichier;

public:
    // constructeurs
    explicit FileWriter(const std::string& path);

    ~FileWriter();

    FileWriter(FileWriter const&) = delete;
    FileWriter& operator=(FileWriter const&) = delete;

    std::ostream& get_flot() const; // pour écrire du texte
};
