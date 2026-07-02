#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "models/User.h"

class Employee : public User {
public:
    static int employeeNextId;

    Employee();
    Employee(const std::string& namenew, const std::string& passwordnew);

    bool canRent() const;

    // We removed rentCar and returnCar as they belong in a RentalService

    friend std::ostream& operator<<(std::ostream& out, const Employee& emp);
    friend std::istream& operator>>(std::istream& in, Employee& emp);
};

#endif // EMPLOYEE_H
