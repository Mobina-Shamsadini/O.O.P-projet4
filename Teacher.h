#pragma once

#include "Members/Member.h"

class Teacher : public Member {
    static constexpr int MAX_BOOKS = 3;

public:
    Teacher(const std::string &name, int memberId, const std::string &department,
            bool wantsNotifications);

    std::string GetTypeLabel() const override { return "Enseignant"; }
    int GetMaxBooks() const override { return MAX_BOOKS; }
};
