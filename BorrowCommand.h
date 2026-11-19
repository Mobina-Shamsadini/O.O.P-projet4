#pragma once

#include "Interfaces/ICommand.h"

class Book;
class Library;

// Un emprunt devenu objet ; il se defait par un retour.
// Ne possede rien : la bibliotheque, le membre et le livre sont a Library.
class BorrowCommand : public ICommand {
    Library &library_;
    Member &member_;
    Book &book_;

    std::string Summary() const;       // "Marie Tremblay -> Dune"

public:
    BorrowCommand(Library &library, Member &member, Book &book);

    bool Execute(std::string &output) override;
    bool Undo(std::string &output) override;
    std::string Describe() const override;
    const Member &GetMember() const override { return member_; }
};
