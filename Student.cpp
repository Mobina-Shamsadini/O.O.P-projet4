#include "Members/Student.h"

Student::Student(const std::string &name, int memberId, const std::string &program,
                 bool wantsNotifications)
    : Member{name, memberId, program, wantsNotifications} {
}
