#pragma once

#include <string>
#include <vector>

#include "Commands/Cart.h"
#include "Interfaces/IBookObserver.h"
#include "Interfaces/IDisplayable.h"

class Book;

// Classe abstraite : chaque type de membre fixe sa limite d'emprunts.
// Un membre est un observateur (Observer) et possede son panier (Command).
class Member : public IDisplayable, public IBookObserver {
    std::string name_;
    int memberId_;
    std::string affiliation_;
    bool wantsNotifications_;
    std::vector<Book *> borrowedBooks_;     // designe seulement : les livres sont a Library
    Cart cart_;                             // cree et detruit avec le membre

    static int liveCount_;

protected:
    Member(const std::string &name, int memberId, const std::string &affiliation,
           bool wantsNotifications);

public:
    ~Member() override;
    Member(const Member &) = delete;
    Member &operator=(const Member &) = delete;

    const std::string &GetName() const { return name_; }
    bool WantsNotifications() const { return wantsNotifications_; }
    virtual std::string GetTypeLabel() const = 0;
    virtual int GetMaxBooks() const = 0;

    Cart &GetCart() { return cart_; }

    bool CanBorrow() const;
    // true : "deja emprunte par ce membre" ; false : "aucun emprunt".
    bool HasBorrowed(const Book &book, std::string &output) const;

    bool Borrow(Book &book, std::string &output);
    bool Return(Book &book, std::string &output);

    void OnBookAvailable(const Book &book) override;
    void Display() const override;

    static int GetLiveCount() { return liveCount_; }
};
