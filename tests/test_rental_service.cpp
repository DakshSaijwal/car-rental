#include <gtest/gtest.h>
#include "models/Manager.h"
#include "services/RentalService.h"
#include "models/Exceptions.h"

class RentalServiceTest : public ::testing::Test {
protected:
    Manager mgr;
    RentalService* rentalService;

    void SetUp() override {
        rentalService = new RentalService(mgr);
        // Setup initial state
        Car c1("Toyota", "Camry", "White", "G", 2020, 100.0);
        mgr.addCar(c1);
        
        Employee e1("Alice", "password123");
        mgr.addEmployee(e1);
        
        Customer cst1("Bob", "password123");
        mgr.addCustomer(cst1);
    }

    void TearDown() override {
        delete rentalService;
    }
};

TEST_F(RentalServiceTest, CalculateDays) {
    EXPECT_EQ(RentalService::calculateDays(20230101, 20230110), 9);
    EXPECT_EQ(RentalService::calculateDays(20230228, 20230301), 1); // Non-leap year
    EXPECT_EQ(RentalService::calculateDays(20240228, 20240301), 2); // Leap year
}

TEST_F(RentalServiceTest, EmployeeRentCarSuccess) {
    // Car ID is 0 based on static nextId, but let's fetch it properly
    mgr.addCar(Car("Honda", "Civic", "Black", "G", 2019, 80.0));
    
    // We assume the first added employee is E0 and car is 0
    std::string empId = "E0";
    int carId = 0;
    
    EXPECT_NO_THROW(rentalService->employeeRentCar(empId, carId, 20230501, 20230505));
    
    auto emp = mgr.getEmployee(empId);
    auto car = mgr.getCar(carId);
    
    EXPECT_EQ(car->getOwnerId(), empId);
    // 4 days * 100.0 * 0.85 = 340
    EXPECT_EQ(emp->Payable, 340);
}

TEST_F(RentalServiceTest, CustomerRentCarFailureWhenCarNotAvailable) {
    mgr.addCar(Car("Ford", "Focus", "Blue", "G", 2021, 90.0));
    std::string empId = "E0";
    std::string custId = "C0";
    int carId = 0;

    // Rent to employee first
    rentalService->employeeRentCar(empId, carId, 20230501, 20230505);
    
    // Customer tries to rent same car
    EXPECT_THROW(rentalService->customerRentCar(custId, carId, 20230502, 20230510), ValidationException);
}

TEST_F(RentalServiceTest, EmployeeReturnCarWithPenalty) {
    std::string empId = "E0";
    int carId = 0;
    
    rentalService->employeeRentCar(empId, carId, 20230501, 20230505); // Due 0505
    rentalService->employeeReturnCar(empId, carId, 20230507); // Returned 2 days late
    
    auto emp = mgr.getEmployee(empId);
    auto car = mgr.getCar(carId);
    
    EXPECT_EQ(car->getOwnerId(), AVAILABLE_OWNER);
    
    // Initial rental cost: 4 days * 100 * 0.85 = 340
    // Penalty: 2 days late * 100 * 1.1 = 220
    // Total Payable = 560
    EXPECT_EQ(emp->Payable, 560);
    EXPECT_EQ(emp->record, -20); // 10 * 2 days late
}

TEST_F(RentalServiceTest, CustomerReturnCarEarlyBonus) {
    std::string custId = "C0";
    int carId = 0;
    
    rentalService->customerRentCar(custId, carId, 20230501, 20230510); // Due 0510
    
    // Customer record initially is avg rating (which is 0 initially)
    rentalService->customerReturnCar(custId, carId, 20230508); // 2 days early
    
    auto cust = mgr.getCustomer(custId);
    EXPECT_EQ(cust->record, 20); // +10 per day early
}
