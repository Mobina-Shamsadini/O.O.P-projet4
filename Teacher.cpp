#include "Members/Teacher.h"

Teacher::Teacher(const std::string &name, int memberId, const std::string &department,
                 bool wantsNotifications)
    : Member{name, memberId, department, wantsNotifications} {
}
