#pragma once

#include <string>

class Member;

// Command - une operation devenue objet : elle peut attendre dans un
// panier, etre executee, puis etre defaite.
class ICommand {
public:
    virtual ~ICommand() = default;

    // Fait l'operation et la journalise. false : refusee, rien n'a change.
    virtual bool Execute(std::string &output) = 0;

    // Defait une operation deja executee. false : impossible (un autre
    // membre a pu prendre le livre entre-temps).
    virtual bool Undo(std::string &output) = 0;

    // "emprunter Dune" : le texte que le panier journalise.
    virtual std::string Describe() const = 0;

    // Le membre concerne : seul son panier accepte cette commande.
    virtual const Member &GetMember() const = 0;
};
