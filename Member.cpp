#include "Members/Member.h"

#include <algorithm>
#include <iostream>

#include "Books/Book.h"
#include "LibraryLogger.h"

int Member::liveCount_ = 0;

// *this : le membre se donne lui-meme comme proprietaire de son panier.
Member::Member(const std::string &name, int memberId, const std::string &affiliation,
               bool wantsNotifications)
    : name_{name}, memberId_{memberId}, affiliation_{affiliation},
      wantsNotifications_{wantsNotifications}, cart_{*this} {
    liveCount_++;
}

Member::~Member() {
    liveCount_--;
}

bool Member::CanBorrow() const {
    return static_cast<int>(borrowedBooks_.size()) < GetMaxBooks();
}

bool Member::HasBorrowed(const Book &book, std::string &output) const {
    if (std::find(borrowedBooks_.begin(), borrowedBooks_.end(), &book) == borrowedBooks_.end()) {
        output = "aucun emprunt";
        return false;
    }
    output = "deja emprunte par ce membre";
    return true;
}

// Les cas d'echec d'abord, dans l'ordre ou le comptoir les verifie.
bool Member::Borrow(Book &book, std::string &output) {
    if (HasBorrowed(book, output))
        return false;
    if (!CanBorrow()) {
        output = "limite atteinte";
        return false;
    }
    std::string detail;
    if (!book.Checkout(detail)) {
        output = book.GetStatusLabel();
        return false;
    }
    borrowedBooks_.push_back(&book);
    output = name_ + " a " + std::to_string(borrowedBooks_.size()) + " livre(s) sur "
             + std::to_string(GetMaxBooks()) + " (" + detail + ")";
    return true;
}

bool Member::Return(Book &book, std::string &output) {
    if (!HasBorrowed(book, output))
        return false;
    if (!book.Return(output))
        return false;
    std::erase(borrowedBooks_, &book);
    return true;
}

void Member::OnBookAvailable(const Book &book) {
    LibraryLogger::GetInstance().Log("[NOTIFICATION] " + name_ + " : " + book.GetTitle() + " disponible");
}

void Member::Display() const {
    std::cout << "  [" << GetTypeLabel() << "] " << name_ << " #" << memberId_
              << ", " << affiliation_ << ", avis " << (wantsNotifications_ ? "oui" : "non")
              << ", " << borrowedBooks_.size() << "/" << GetMaxBooks() << " livre(s)";
    for (const Book *book: borrowedBooks_)
        std::cout << " | " << book->GetTitle();
    std::cout << "\n";
}
