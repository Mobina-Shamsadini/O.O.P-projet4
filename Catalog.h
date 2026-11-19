#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Conteneur generique, proprietaire de ses elements (livres ou membres).
// Template : tout le code est dans le .h, aucun .cpp.
//
// main.cpp parcourt GetItems() avec des std::shared_ptr : le catalogue les
// garde donc ainsi, mais il en reste le seul proprietaire. Les autres classes
// ne recoivent que des pointeurs bruts ou des references, qui designent
// sans posseder.
template<typename T>
class Catalog {
    std::vector<std::shared_ptr<T>> items_;

public:
    // Le catalogue devient proprietaire de l'element recu.
    void Add(std::unique_ptr<T> item) {
        items_.push_back(std::move(item));
    }

    int Count() const {
        return static_cast<int>(items_.size());
    }

    const std::vector<std::shared_ptr<T>> &GetItems() const {
        return items_;
    }

    // L'element a cet indice, ou nullptr si l'indice est hors limites.
    T *Get(int index, std::string &output) const {
        const std::string position = "indice " + std::to_string(index);
        if (index < 0 || index >= Count()) {
            output = position + " hors limites : le catalogue contient "
                     + std::to_string(Count()) + " element(s)";
            return nullptr;
        }
        output = position + " sur " + std::to_string(Count()) + " element(s)";
        return items_[static_cast<std::size_t>(index)].get();
    }

    // Le premier element qui satisfait le predicat, ou nullptr.
    template<typename Predicate>
    T *FindIf(const Predicate &matches) const {
        for (const std::shared_ptr<T> &item: items_) {
            if (matches(*item))
                return item.get();
        }
        return nullptr;
    }

    // Tri par insertion, donc stable : deux elements egaux selon le
    // critere gardent leur ordre. Le critere offre Before(a, b, output).
    template<typename Criterion>
    void Sort(const Criterion &criterion) {
        std::string detail;      // explication de chaque comparaison, inutile ici
        for (std::size_t next = 1; next < items_.size(); next++) {
            for (std::size_t position = next;
                 position > 0 && criterion.Before(*items_[position], *items_[position - 1], detail);
                 position--) {
                std::swap(items_[position], items_[position - 1]);
            }
        }
    }

    void DisplayAll() const {
        for (const std::shared_ptr<T> &item: items_)
            item->Display();
    }
};
