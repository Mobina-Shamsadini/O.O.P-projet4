#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Catalog.h"

class Book;
class IBookObserver;
class ISortStrategy;
class Member;

// La bibliotheque : proprietaire de ses deux catalogues.
// Sujet du patron Observer (une liste d'attente par livre) et Contexte du
// patron Strategy (le critere de tri, qu'elle utilise sans le posseder).
class Library {
    Catalog<Book> books_;
    Catalog<Member> members_;
    // Cle : l'adresse d'un livre du catalogue. Valeur : ceux qui l'attendent.
    // Ni les livres ni les observateurs n'appartiennent a cette table.
    std::map<const Book *, std::vector<IBookObserver *>> waitLists_;
    const ISortStrategy *sortStrategy_ = nullptr;

    void Subscribe(const Book &book, IBookObserver &observer);
    void NotifyAvailable(const Book &book);

public:
    void AddBook(std::unique_ptr<Book> book);
    void AddMember(std::unique_ptr<Member> member);

    int GetBookCount() const { return books_.Count(); }
    int GetMemberCount() const { return members_.Count(); }
    const Catalog<Book> &GetBooks() const { return books_; }

    // Ces pointeurs designent un objet du catalogue, sans le ceder.
    Book *FindBook(const std::string &title) const;
    Member *FindMember(const std::string &name) const;

    // La strategie doit vivre plus longtemps que la bibliotheque.
    void SetSortStrategy(const ISortStrategy &strategy) { sortStrategy_ = &strategy; }
    void SortBooks();
    void DisplayCatalog() const;
    void DisplayMembers() const;

    bool BorrowBook(Member &member, Book &book, std::string &output);
    bool ReturnBook(Member &member, Book &book, std::string &output);

    void Unsubscribe(const Book &book, const IBookObserver &observer);
    bool IsWaiting(const Book &book, const IBookObserver &observer, std::string &output) const;
};
