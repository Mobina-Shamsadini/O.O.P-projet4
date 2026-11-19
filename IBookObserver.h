#pragma once

class Book;

// Observer - l'Observateur : le seul contrat que Library connait pour
// prevenir quelqu'un qu'un livre attendu est revenu.
class IBookObserver {
public:
    virtual ~IBookObserver() = default;

    virtual void OnBookAvailable(const Book &book) = 0;
};
