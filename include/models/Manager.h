#ifndef MANAGER_H
#define MANAGER_H

#include "models/Car.h"
#include "models/Employee.h"
#include "models/Customer.h"
#include "models/Exceptions.h"
#include <unordered_map>
#include <string>
#include <memory>

class Manager {
private:
    std::unordered_map<int, std::shared_ptr<Car>> cars;
    std::unordered_map<std::string, std::shared_ptr<Employee>> employees;
    std::unordered_map<std::string, std::shared_ptr<Customer>> customers;
    std::string password;

public:
    Manager();

    // Passwords
    bool checkPassword(const std::string& pass) const;
    void setPassword(const std::string& newPass);

    // Car Management
    void addCar(const Car& car);
    void deleteCar(int carId);
    std::shared_ptr<Car> getCar(int carId) const;
    void displayAllCars() const;
    void showAvailableCars(int date) const;

    // Employee Management
    void addEmployee(const Employee& emp);
    void deleteEmployee(const std::string& id);
    std::shared_ptr<Employee> getEmployee(const std::string& id) const;
    void displayAllEmployees() const;

    // Customer Management
    void addCustomer(const Customer& cust);
    void deleteCustomer(const std::string& id);
    std::shared_ptr<Customer> getCustomer(const std::string& id) const;
    void displayAllCustomers() const;

    // Persistence
    void loadData();
    void saveData() const;
};

#endif // MANAGER_H
