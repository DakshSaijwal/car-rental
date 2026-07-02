#include "models/Car.h"

int Car::nextId = 0;

Car::Car() {
    id = -1;
    make = "";
    model = "";
    color = "";
    condition = "";
    year = 0;
    dailyRate = 0;
    ownerId = AVAILABLE_OWNER;
    dueDate = 0;
}

Car::Car(std::string make, std::string model, std::string color, std::string condition, int year, double dailyRate) {
    this->id = nextId++;
    this->make = make;
    this->model = model;
    this->color = color;
    this->condition = condition;
    this->year = year;
    this->dailyRate = dailyRate;
    ownerId = AVAILABLE_OWNER;
    dueDate = 0;
}

void Car::display() const {
    std::cout << "Car ID: " << id << "\n";
    std::cout << "Make: " << make << "\n";
    std::cout << "Model: " << model << "\n";
    std::cout << "Color: " << color << "\n";
    std::cout << "Condition: " << condition << "\n";
    std::cout << "Year: " << year << "\n";
    std::cout << "Daily Rate: " << dailyRate << "\n";
    if (ownerId == AVAILABLE_OWNER) {
        std::cout << "Status: Available for rent.\n";
    } else {
        std::cout << "Status: Rented by " << ownerId << "\n";
        std::cout << "Due date: " << dueDate << "\n";
    }
    std::cout << "------------------------\n";
}

void Car::RentRequest(std::string u_id, int dueDate) {
    ownerId = u_id;
    this->dueDate = dueDate;
}

int Car::getId() const {
    return id;
}

void Car::setDueDate(int dueDate) {
    this->dueDate = dueDate;
}

int Car::getDueDate() const {
    return dueDate;
}

bool Car::isAvailable(int date) const {
    if (ownerId == AVAILABLE_OWNER) {
        return true;
    }
    if (date > dueDate) {
        return true; // Technically overdue, but logic left intact for now
    }
    return false;
}

std::string Car::getOwnerId() const {
    return ownerId;
}

double Car::getDailyRate() const {
    return dailyRate;
}

std::ostream& operator<<(std::ostream& os, const Car& car) {
    os << car.id << " " << car.make << " " << car.model << " " << car.color << " " 
       << car.condition << " " << car.year << " " << car.dailyRate << " " 
       << car.ownerId << " " << car.dueDate << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, Car& car) {
    is >> car.id >> car.make >> car.model >> car.color >> car.condition >> car.year >> car.dailyRate >> car.ownerId >> car.dueDate;
    return is;
}
