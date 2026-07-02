#include "models/Employee.h"
#include <iostream>

int Employee::employeeNextId = 0;

Employee::Employee() : User() {
    position = "Employee";
}

Employee::Employee(const std::string& namenew, const std::string& passwordnew) {
    name = namenew;
    password = std::to_string(std::hash<std::string>{}(passwordnew));
    position = "Employee";
    id = "E" + std::to_string(employeeNextId++);
    if (namenew.empty() || passwordnew.empty()) {
        employeeNextId--;
    }
    // We don't push back -1 to cars vector here. We'll handle it cleaner in User.
    // In original code, there was a -1 sentinel.
    numberofCars = 0;
    record = 0;
    Payable = 0;
}

bool Employee::canRent() const {
    if (record <= -100) {
        std::cout << "Bad record.\n";
        return false;
    }
    int limit;
    if (record >= 50) {
        limit = 10;
    } else if (record >= -20) {
        limit = (record + 40) / 10;
    } else {
        limit = 1;
    }
    
    if (this->numberofCars >= limit) {
        std::cout << "Maximum cars rented.\n";
        return false;
    }
    if (this->Payable >= 150000) {
        std::cout << "Payable limit reached.\n";
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    out << emp.id << ' ' << emp.name << ' ' << emp.password << ' ' << emp.position << ' ' 
        << emp.record << ' ' << emp.Payable << ' ' << emp.numberofCars;
    for (int carId : emp.cars) {
        out << ' ' << carId;
    }
    out << " -1\n"; // Sentinel for loading backwards compat
    return out;
}

std::istream& operator>>(std::istream& in, Employee& emp) {
    in >> emp.id >> emp.name >> emp.password >> emp.position >> emp.record >> emp.Payable >> emp.numberofCars;
    int carId;
    emp.cars.clear();
    while (in >> carId && carId != -1) {
        emp.cars.push_back(carId);
    }
    return in;
}
