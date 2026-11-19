#include "Commands/BorrowCommand.h"

#include "Books/Book.h"
#include "Library.h"
#include "LibraryLogger.h"
#include "Members/Member.h"

BorrowCommand::BorrowCommand(Library &library, Member &member, Book &book)
    : library_{library}, member_{member}, book_{book} {
}

std::string BorrowCommand::Summary() const {
    return member_.GetName() + " -> " + book_.GetTitle();
}

std::string BorrowCommand::Describe() const {
    return "emprunter " + book_.GetTitle();
}

// Library emprunte, ou inscrit le membre sur la liste d'attente ; la commande
// journalise le resultat ensuite, donc apres l'eventuelle ligne [ETAT].
bool BorrowCommand::Execute(std::string &output) {
    std::string reason;
    const bool borrowed = library_.BorrowBook(member_, book_, reason);
    output = borrowed ? "[EMPRUNT] " + Summary() : "[REFUS] " + Summary() + " (" + reason + ")";
    LibraryLogger::GetInstance().Log(output);
    return borrowed;
}

// Defaire un emprunt, c'est rendre le livre. [ANNULATION] doit preceder les
// lignes [ETAT] et [NOTIFICATION] du retour : on verifie donc d'abord que
// le retour sera accepte, on journalise, puis on rend.
bool BorrowCommand::Undo(std::string &output) {
    std::string reason;
    if (!member_.HasBorrowed(book_, reason)) {
        output = "[REFUS] annulation : " + member_.GetName() + " ne peut pas rendre "
                 + book_.GetTitle() + " (" + reason + ")";
        LibraryLogger::GetInstance().Log(output);
        return false;
    }
    output = "[ANNULATION] " + member_.GetName() + " rend " + book_.GetTitle();
    LibraryLogger::GetInstance().Log(output);
    return library_.ReturnBook(member_, book_, reason);
}
