#include "Commands/ReturnCommand.h"

#include "Books/Book.h"
#include "Library.h"
#include "LibraryLogger.h"
#include "Members/Member.h"

ReturnCommand::ReturnCommand(Library &library, Member &member, Book &book)
    : library_{library}, member_{member}, book_{book} {
}

std::string ReturnCommand::Summary() const {
    return member_.GetName() + " <- " + book_.GetTitle();
}

std::string ReturnCommand::Describe() const {
    return "rendre " + book_.GetTitle();
}

// [RETOUR] doit preceder les lignes [ETAT] et [NOTIFICATION] que le retour
// provoque : on verifie d'abord que le membre a bien ce livre, on
// journalise, puis Library fait le retour et previent la liste d'attente.
bool ReturnCommand::Execute(std::string &output) {
    std::string reason;
    if (!member_.HasBorrowed(book_, reason)) {
        output = "[REFUS] " + Summary() + " (" + reason + ")";
        LibraryLogger::GetInstance().Log(output);
        return false;
    }
    output = "[RETOUR] " + Summary();
    LibraryLogger::GetInstance().Log(output);
    return library_.ReturnBook(member_, book_, reason);
}

// Defaire un retour, c'est reprendre le livre. Si un autre membre l'a pris,
// Library refuse et inscrit le membre sur la liste d'attente s'il le souhaite.
bool ReturnCommand::Undo(std::string &output) {
    std::string reason;
    if (library_.BorrowBook(member_, book_, reason)) {
        output = "[ANNULATION] " + member_.GetName() + " reprend " + book_.GetTitle();
        LibraryLogger::GetInstance().Log(output);
        return true;
    }
    output = "[REFUS] annulation : " + member_.GetName() + " ne peut pas reprendre "
             + book_.GetTitle() + " (" + reason + ")";
    LibraryLogger::GetInstance().Log(output);
    return false;
}
