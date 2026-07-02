#include "models/Customer.h"
#include <iostream>

int Customer::customerNextId = 0;
int Customer::avgcustomerRating = 0;

Customer::Customer() : User() {
    position = "Customer";
}

Customer::Customer(const std::string& namenew, const std::string& passwordnew) {
    name = namenew;
    password = std::to_string(std::hash<std::string>{}(passwordnew));
    position = "Customer";
    id = "C" + std::to_string(customerNextId++);
    if (namenew.empty() || passwordnew.empty()) {
        customerNextId--;
    }
    numberofCars = 0;
    record = avgcustomerRating;
    Payable = 0;
}

bool Customer::canRent() const {
    if (record <= -50) {
        std::cout << "Bad record.\n";
        return false;
    }
    int limit;
    if (record >= 40) {
        limit = 7;
    } else if (record >= -20) {
        limit = (record + 40) / 10;
    } else {
        limit = 1;
    }
    if (this->numberofCars >= limit) {
        std::cout << "Maximum cars rented.\n";
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Customer& cst) {
    out << cst.id << ' ' << cst.name << ' ' << cst.password << ' ' << cst.position << ' ' 
        << cst.record << ' ' << cst.Payable << ' ' << cst.numberofCars;
    for (int carId : cst.cars) {
        out << ' ' << carId;
    }
    out << " -1\n";
    return out;
}

std::istream& operator>>(std::istream& in, Customer& cst) {
    in >> cst.id >> cst.name >> cst.password >> cst.position >> cst.record >> cst.Payable >> cst.numberofCars;
    int carId;
    cst.cars.clear();
    while (in >> carId && carId != -1) {
        cst.cars.push_back(carId);
    }
    return in;
}
