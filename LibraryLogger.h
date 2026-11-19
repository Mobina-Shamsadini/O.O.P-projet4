#pragma once

#include <fstream>
#include <string>

// Singleton : le journal unique de la bibliotheque (library.log).
// Chaque ligne recoit un numero ; la numerotation ne recommence jamais.
class LibraryLogger {
    std::ofstream file_;
    int lineCount_ = 0;

    LibraryLogger();                       // regle 1 : constructeur prive

public:
    LibraryLogger(const LibraryLogger &) = delete;             // regle 2 :
    LibraryLogger &operator=(const LibraryLogger &) = delete;  // copie interdite

    static LibraryLogger &GetInstance();   // regle 3 : point d'acces statique

    // Ecrit "[n] message" dans library.log et dans la console.
    void Log(const std::string &message);
};
