#ifndef RENTAL_SERVICE_H
#define RENTAL_SERVICE_H

#include "models/Manager.h"
#include <string>

class RentalService {
private:
    Manager& manager;

public:
    explicit RentalService(Manager& mgr);

    // Employee Renting
    void employeeRentCar(const std::string& empId, int carId, int startDate, int dueDate);
    void employeeReturnCar(const std::string& empId, int carId, int currentDate);

    // Customer Renting
    void customerRentCar(const std::string& custId, int carId, int startDate, int dueDate);
    void customerReturnCar(const std::string& custId, int carId, int currentDate);
    
    // For Stage 3: Date handling helper
    static int calculateDays(int startDate, int endDate); 
};

#endif // RENTAL_SERVICE_H
