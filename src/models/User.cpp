#include "models/User.h"
#include <algorithm>

User::User() {
    name = "";
    password = "";
    position = "";
    record = 0;
    Payable = 0;
    numberofCars = 0;
}

std::string User::getname() const {
    return name;
}

std::string User::getpass() const {
    return password;
}

std::string User::getPosition() const {
    return position;
}

std::string User::getId() const {
    return id;
}

bool User::checkpass(const std::string& pass) const {
    return password == std::to_string(std::hash<std::string>{}(pass));
}

void User::addCar(int carId) {
    cars.push_back(carId);
    numberofCars++;
}

void User::removeCar(int carId) {
    auto it = std::find(cars.begin(), cars.end(), carId);
    if (it != cars.end()) {
        cars.erase(it);
        numberofCars--;
    } else {
        std::cout << "Car not found.\n";
    }
}

void User::showcars() const {
    if (numberofCars == 0) {
        std::cout << "No cars found.\n";
    }
    for (int i : cars) {
        std::cout << "Car ID: " << i << "\n";
    }
}

void User::clear_due() {
    std::cout << "Amount to be paid: " << Payable << "\n";
    Payable = 0;
    std::cout << "Due cleared.\n";
}

void User::display() const {
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Position: " << position << "\n";
    std::cout << "Record: " << record << "\n";
    std::cout << "Payable: " << Payable << "\n";
    std::cout << "Number of Cars: " << numberofCars << "\n";
    std::cout << "Cars rented: ";
    for (int i : cars) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    std::cout << "------------------------\n";
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.id << " " << user.name << " " << user.password << " " << user.position << " " 
       << user.record << " " << user.Payable << " " << user.numberofCars;
    for (int carId : user.cars) {
        os << " " << carId;
    }
    os << " -1\n"; // Sentinel value
    return os;
}

std::istream& operator>>(std::istream& is, User& user) {
    is >> user.id >> user.name >> user.password >> user.position >> user.record >> user.Payable >> user.numberofCars;
    int carId;
    user.cars.clear();
    while (is >> carId && carId != -1) {
        user.cars.push_back(carId);
    }
    return is;
}
