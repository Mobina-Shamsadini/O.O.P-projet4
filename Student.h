#pragma once

#include "Members/Member.h"

class Student : public Member {
    static constexpr int MAX_BOOKS = 2;

public:
    Student(const std::string &name, int memberId, const std::string &program,
            bool wantsNotifications);

    std::string GetTypeLabel() const override { return "Etudiant"; }
    int GetMaxBooks() const override { return MAX_BOOKS; }
};
