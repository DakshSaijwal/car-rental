#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "models/User.h"

class Customer : public User {
public:
    static int customerNextId;
    static int avgcustomerRating;

    Customer();
    Customer(const std::string& namenew, const std::string& passwordnew);

    bool canRent() const;

    friend std::ostream& operator<<(std::ostream& out, const Customer& cst);
    friend std::istream& operator>>(std::istream& in, Customer& cst);
};

#endif // CUSTOMER_H
