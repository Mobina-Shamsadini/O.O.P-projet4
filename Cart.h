#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Interfaces/ICommand.h"

class Member;

// Command - l'Invocateur : le panier d'un membre. Il possede ses commandes
// en attente et l'historique des ordres deja valides.
class Cart {
    const Member &owner_;
    std::vector<std::unique_ptr<ICommand>> pending_;
    // Un ordre = les commandes reussies d'une validation (ou d'un emprunt
    // immediat). Le plus recent est au bout.
    std::vector<std::vector<std::unique_ptr<ICommand>>> orders_;

    bool Accepts(const ICommand &command, std::string &output) const;
    void Keep(std::vector<std::unique_ptr<ICommand>> order);
    // Ecrit "[TAG] Nom : detail" dans output, puis dans le journal.
    void Record(const std::string &tag, const std::string &detail, std::string &output) const;

public:
    explicit Cart(const Member &owner);
    Cart(const Cart &) = delete;
    Cart &operator=(const Cart &) = delete;

    bool Add(std::unique_ptr<ICommand> command, std::string &output);
    bool Cancel(std::string &output);
    int Checkout(std::string &output);
    bool ExecuteNow(std::unique_ptr<ICommand> command, std::string &output);
    bool Undo(std::string &output);
};
