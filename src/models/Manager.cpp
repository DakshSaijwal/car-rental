#include "models/Manager.h"
#include "persistence/FileRepository.h"
#include <iostream>
#include <algorithm>

Manager::Manager() {
    password = std::to_string(std::hash<std::string>{}("Mrental001"));
}

bool Manager::checkPassword(const std::string& pass) const {
    return password == std::to_string(std::hash<std::string>{}(pass));
}

void Manager::setPassword(const std::string& newPass) {
    password = std::to_string(std::hash<std::string>{}(newPass));
}

void Manager::addCar(const Car& car) {
    cars[car.getId()] = std::make_shared<Car>(car);
}

void Manager::deleteCar(int carId) {
    if (cars.erase(carId) == 0) {
        throw NotFoundException("Car not found.");
    }
}

std::shared_ptr<Car> Manager::getCar(int carId) const {
    auto it = cars.find(carId);
    if (it != cars.end()) {
        return it->second;
    }
    throw NotFoundException("Car not found.");
}

void Manager::displayAllCars() const {
    if (cars.empty()) {
        std::cout << "No cars found.\n";
    }
    for (const auto& pair : cars) {
        pair.second->display();
    }
}

void Manager::showAvailableCars(int date) const {
    int counter = 0;
    for (const auto& pair : cars) {
        if (pair.second->isAvailable(date)) {
            pair.second->display();
            counter++;
        }
    }
    if (counter == 0) {
        std::cout << "No cars available at the given date. Sorry for your inconvenience!\n";
    }
    std::cout << std::endl;
}

void Manager::addEmployee(const Employee& emp) {
    employees[emp.getId()] = std::make_shared<Employee>(emp);
}

void Manager::deleteEmployee(const std::string& id) {
    if (employees.erase(id) == 0) {
        throw NotFoundException("Employee not found.");
    }
}

std::shared_ptr<Employee> Manager::getEmployee(const std::string& id) const {
    auto it = employees.find(id);
    if (it != employees.end()) {
        return it->second;
    }
    throw NotFoundException("Employee not found.");
}

void Manager::displayAllEmployees() const {
    if (employees.empty()) {
        std::cout << "No employees found.\n";
    }
    for (const auto& pair : employees) {
        pair.second->display();
    }
}

void Manager::addCustomer(const Customer& cust) {
    customers[cust.getId()] = std::make_shared<Customer>(cust);
}

void Manager::deleteCustomer(const std::string& id) {
    auto it = customers.find(id);
    if (it != customers.end()) {
        std::shared_ptr<Customer> cust = it->second;
        for (int carId : cust->cars) {
            try {
                auto carobj = getCar(carId);
                carobj->RentRequest(AVAILABLE_OWNER, 0);
            } catch (const NotFoundException&) {
                // Ignore missing cars during cascade delete
            }
        }
        customers.erase(it);
    } else {
        throw NotFoundException("Customer not found.");
    }
}

std::shared_ptr<Customer> Manager::getCustomer(const std::string& id) const {
    auto it = customers.find(id);
    if (it != customers.end()) {
        return it->second;
    }
    throw NotFoundException("Customer not found.");
}

void Manager::displayAllCustomers() const {
    if (customers.empty()) {
        std::cout << "No customers found.\n";
    }
    for (const auto& pair : customers) {
        pair.second->display();
    }
}

void Manager::loadData() {
    FileRepository<int, Car>::load("cars.txt", cars, [](const Car& c) {
        Car::nextId = std::max(Car::nextId, c.getId() + 1);
    });
    
    FileRepository<std::string, Employee>::load("employees.txt", employees, [](const Employee& e) {
        if (e.getId().length() > 1) {
            Employee::employeeNextId = std::max(Employee::employeeNextId, std::stoi(e.getId().substr(1)) + 1);
        }
    });
    
    FileRepository<std::string, Customer>::load("customers.txt", customers, [](const Customer& c) {
        if (c.getId().length() > 1) {
            Customer::customerNextId = std::max(Customer::customerNextId, std::stoi(c.getId().substr(1)) + 1);
        }
        Customer::avgcustomerRating += c.record;
    });
    
    if (!customers.empty()) {
        Customer::avgcustomerRating /= static_cast<int>(customers.size());
    }
}

void Manager::saveData() const {
    FileRepository<int, Car>::save("cars.txt", cars);
    FileRepository<std::string, Employee>::save("employees.txt", employees);
    FileRepository<std::string, Customer>::save("customers.txt", customers);
}
