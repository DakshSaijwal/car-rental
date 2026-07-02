#include "services/RentalService.h"
#include "models/Exceptions.h"
#include <iostream>
#include <ctime>

RentalService::RentalService(Manager& mgr) : manager(mgr) {}

int RentalService::calculateDays(int start, int end) {
    auto to_time_t = [](int dateInt) -> time_t {
        std::tm tm = {};
        tm.tm_year = (dateInt / 10000) - 1900;
        tm.tm_mon = ((dateInt / 100) % 100) - 1;
        tm.tm_mday = dateInt % 100;
        tm.tm_hour = 12; // Noon to avoid daylight saving issues
        return std::mktime(&tm);
    };
    
    time_t t1 = to_time_t(start);
    time_t t2 = to_time_t(end);
    
    double diff = std::difftime(t2, t1);
    return static_cast<int>(diff / (60 * 60 * 24));
}

void RentalService::employeeRentCar(const std::string& empId, int carId, int startDate, int dueDate) {
    auto emp = manager.getEmployee(empId);
    auto car = manager.getCar(carId);
    
    if (!car->isAvailable(startDate)) {
        throw ValidationException("Car is not available.");
    }
    if (!emp->canRent()) {
        throw ValidationException("Employee cannot rent more cars.");
    }
    
    car->RentRequest(emp->getId(), dueDate);
    emp->addCar(carId);
    
    int days = calculateDays(startDate, dueDate);
    if (days < 0) days = 0; // Guard against negative dates
    
    emp->Payable += static_cast<int>(car->getDailyRate() * days * 0.85); // 15% employee discount
}

void RentalService::employeeReturnCar(const std::string& empId, int carId, int currentDate) {
    auto emp = manager.getEmployee(empId);
    auto car = manager.getCar(carId);
    
    if (car->getOwnerId() != empId) {
        throw ValidationException("Car is not rented by you.");
    }
    
    if (car->getDueDate() < currentDate) {
        int daysLate = calculateDays(car->getDueDate(), currentDate);
        emp->Payable += static_cast<int>(car->getDailyRate() * daysLate * 1.1); // 10% penalty
        emp->record -= 10 * daysLate;
    } else if (car->getDueDate() > currentDate) {
        int daysEarly = calculateDays(currentDate, car->getDueDate());
        emp->record += 10 * daysEarly;
    }
    
    car->RentRequest(AVAILABLE_OWNER, 0);
    emp->removeCar(carId);
}

void RentalService::customerRentCar(const std::string& custId, int carId, int startDate, int dueDate) {
    auto cust = manager.getCustomer(custId);
    auto car = manager.getCar(carId);
    
    if (!car->isAvailable(startDate)) {
        throw ValidationException("Car is not available.");
    }
    if (!cust->canRent()) {
        throw ValidationException("Customer cannot rent more cars.");
    }
    
    car->RentRequest(cust->getId(), dueDate);
    cust->addCar(carId);
    
    int days = calculateDays(startDate, dueDate);
    if (days < 0) days = 0;
    
    cust->Payable += static_cast<int>(car->getDailyRate() * days);
}

void RentalService::customerReturnCar(const std::string& custId, int carId, int currentDate) {
    auto cust = manager.getCustomer(custId);
    auto car = manager.getCar(carId);
    
    if (car->getOwnerId() != custId) {
        throw ValidationException("Car is not rented by you.");
    }
    
    if (car->getDueDate() < currentDate) {
        int daysLate = calculateDays(car->getDueDate(), currentDate);
        cust->Payable += static_cast<int>(car->getDailyRate() * daysLate * 1.25); // 25% penalty
        cust->record -= 10 * daysLate;
    } else if (car->getDueDate() > currentDate) {
        int daysEarly = calculateDays(currentDate, car->getDueDate());
        cust->record += 10 * daysEarly;
    }
    
    car->RentRequest(AVAILABLE_OWNER, 0);
    cust->removeCar(carId);
}
