#include "models/Manager.h"
#include "services/RentalService.h"
#include "models/Exceptions.h"
#include <iostream>
#include <limits>
#include <string>

// Robust input handlers
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cout << "Invalid input. Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

bool isValidDate(int date) {
    if (date < 19000000 || date > 21000000) return false;
    int m = (date / 100) % 100;
    int d = date % 100;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    return true;
}

int readDate(const std::string& prompt) {
    while (true) {
        int date = readInt(prompt);
        if (isValidDate(date)) {
            return date;
        }
        std::cout << "Invalid date format. Expected YYYYMMDD.\n";
    }
}

void managerMenu(Manager& mgr) {
    std::cout << "Welcome, Manager.\n";
    while (true) {
        std::cout << "\nManager Menu:\n"
                  << "0. Change Password\n"
                  << "1. Add a car\n"
                  << "2. Delete a car\n"
                  << "3. Display all cars\n"
                  << "4. Add an employee\n"
                  << "5. Delete an employee\n"
                  << "6. Display all employees\n"
                  << "7. Add a customer\n"
                  << "8. Delete a customer\n"
                  << "9. Display all customers\n"
                  << "10. Exit\n";
        int choice = readInt("Enter your choice: ");
        
        if (choice == 10) break;
        
        try {
            switch (choice) {
                case 0: {
                    std::string newPass = readString("Enter the new password: ");
                    while (newPass.length() < 8) {
                        std::cout << "Password must be at least 8 characters long.\n";
                        newPass = readString("Enter the new password: ");
                    }
                    mgr.setPassword(newPass);
                    std::cout << "Password changed successfully.\n";
                    break;
                }
                case 1: {
                    std::string make = readString("Enter the make of the car: ");
                    std::string model = readString("Enter the model of the car: ");
                    std::string color = readString("Enter the color of the car: ");
                    std::string condition = readString("Enter the condition of the car (G: Good, B: Bad, D: Doable): ");
                    int year = readInt("Enter the year of the car: ");
                    double rate = readInt("Enter the daily rate of the car: ");
                    mgr.addCar(Car(make, model, color, condition, year, rate));
                    std::cout << "Car added successfully.\n";
                    break;
                }
                case 2: {
                    int id = readInt("Enter the id of the car to delete: ");
                    mgr.deleteCar(id);
                    break;
                }
                case 3: mgr.displayAllCars(); break;
                case 4: {
                    std::string name = readString("Enter the name of the employee: ");
                    std::string pass = readString("Enter the password of the employee: ");
                    while (pass.length() < 8) {
                        std::cout << "Password must be at least 8 characters long.\n";
                        pass = readString("Enter the password of the employee: ");
                    }
                    mgr.addEmployee(Employee(name, pass));
                    std::cout << "Employee added successfully.\n";
                    break;
                }
                case 5: {
                    std::string id = readString("Enter the id of the employee to delete: ");
                    mgr.deleteEmployee(id);
                    break;
                }
                case 6: mgr.displayAllEmployees(); break;
                case 7: {
                    std::string name = readString("Enter the name of the customer: ");
                    std::string pass = readString("Enter the password of the customer: ");
                    while (pass.length() < 8) {
                        std::cout << "Password must be at least 8 characters long.\n";
                        pass = readString("Enter the password of the customer: ");
                    }
                    mgr.addCustomer(Customer(name, pass));
                    std::cout << "Customer added successfully.\n";
                    break;
                }
                case 8: {
                    std::string id = readString("Enter the id of the customer to delete: ");
                    mgr.deleteCustomer(id);
                    break;
                }
                case 9: mgr.displayAllCustomers(); break;
                default: std::cout << "Invalid choice.\n";
            }
        } catch (const RentalException& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void employeeMenu(Manager& mgr, RentalService& rental, const std::string& empId) {
    try {
        auto emp = mgr.getEmployee(empId);
        std::cout << "Welcome, " << emp->getname() << ".\n";
        
        while (true) {
            emp->display();
            std::cout << "\nEmployee Menu:\n"
                      << "1. Rent a car\n"
                      << "2. Return a car\n"
                      << "3. Display all available cars\n"
                      << "4. Display all your cars\n"
                      << "5. Clear due\n"
                      << "6. Exit\n";
            int choice = readInt("Enter your choice: ");
            
            if (choice == 6) break;
            
            try {
                switch (choice) {
                    case 1: {
                        int carId = readInt("Enter the id of the car: ");
                        int startDate = readDate("Enter the start date (YYYYMMDD): ");
                        int dueDate = readDate("Enter the due date (YYYYMMDD): ");
                        while (dueDate < startDate) {
                            std::cout << "Due date must be after start date.\n";
                            dueDate = readDate("Enter the due date (YYYYMMDD): ");
                        }
                        rental.employeeRentCar(emp->getId(), carId, startDate, dueDate);
                        std::cout << "Car rented successfully.\n";
                        break;
                    }
                    case 2: {
                        int carId = readInt("Enter the id of the car: ");
                        int date = readDate("Enter the current date (YYYYMMDD): ");
                        rental.employeeReturnCar(emp->getId(), carId, date);
                        std::cout << "Car returned successfully.\n";
                        break;
                    }
                    case 3: {
                        int date = readDate("Enter the date (YYYYMMDD): ");
                        mgr.showAvailableCars(date);
                        break;
                    }
                    case 4: emp->showcars(); break;
                    case 5: emp->clear_due(); break;
                    default: std::cout << "Invalid choice.\n";
                }
            } catch (const RentalException& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    } catch (const RentalException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void customerMenu(Manager& mgr, RentalService& rental, const std::string& custId) {
    try {
        auto cust = mgr.getCustomer(custId);
        std::cout << "Welcome, " << cust->getname() << ".\n";
        
        while (true) {
            cust->display();
            std::cout << "\nCustomer Menu:\n"
                      << "1. Rent a car\n"
                      << "2. Return a car\n"
                      << "3. Display all available cars\n"
                      << "4. Display all your cars\n"
                      << "5. Clear due\n"
                      << "6. Exit\n";
            int choice = readInt("Enter your choice: ");
            
            if (choice == 6) break;
            
            try {
                switch (choice) {
                    case 1: {
                        int carId = readInt("Enter the id of the car: ");
                        int startDate = readDate("Enter the start date (YYYYMMDD): ");
                        int dueDate = readDate("Enter the due date (YYYYMMDD): ");
                        while (dueDate < startDate) {
                            std::cout << "Due date must be after start date.\n";
                            dueDate = readDate("Enter the due date (YYYYMMDD): ");
                        }
                        rental.customerRentCar(cust->getId(), carId, startDate, dueDate);
                        std::cout << "Car rented successfully.\n";
                        break;
                    }
                    case 2: {
                        int carId = readInt("Enter the id of the car: ");
                        int date = readDate("Enter the current date (YYYYMMDD): ");
                        rental.customerReturnCar(cust->getId(), carId, date);
                        std::cout << "Car returned successfully.\n";
                        break;
                    }
                    case 3: {
                        int date = readDate("Enter the date (YYYYMMDD): ");
                        mgr.showAvailableCars(date);
                        break;
                    }
                    case 4: cust->showcars(); break;
                    case 5: cust->clear_due(); break;
                    default: std::cout << "Invalid choice.\n";
                }
            } catch (const RentalException& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    } catch (const RentalException& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

int main() {
    Manager mgr;
    mgr.loadData();
    RentalService rental(mgr);

    std::cout << "=================================\n";
    std::cout << "  Car Rental Management System   \n";
    std::cout << "=================================\n";

    while (true) {
        std::string position = readString("\nPlease enter your position (Manager, Employee, Customer) or Exit to quit: ");
        
        if (position == "Exit" || position == "exit") {
            break;
        }
        
        if (position == "Manager") {
            std::string pass = readString("Please enter the password: ");
            int counter = 0;
            while (!mgr.checkPassword(pass)) {
                std::cout << "Incorrect password.\n";
                counter++;
                if (counter >= 3) {
                    std::cout << "Too many incorrect attempts. Returning to main menu.\n";
                    break;
                }
                pass = readString("Please enter the password again: ");
            }
            if (mgr.checkPassword(pass)) {
                managerMenu(mgr);
            }
        } 
        else if (position == "Employee") {
            std::string id = readString("Enter your id: ");
            try {
                auto emp = mgr.getEmployee(id);
                std::string pass = readString("Enter your password: ");
                int counter = 0;
                while (!emp->checkpass(pass)) {
                    std::cout << "Incorrect password.\n";
                    counter++;
                    if (counter >= 3) {
                        std::cout << "Too many incorrect attempts. Returning to main menu.\n";
                        break;
                    }
                    pass = readString("Enter your password again: ");
                }
                if (emp->checkpass(pass)) {
                    employeeMenu(mgr, rental, id);
                }
            } catch (const NotFoundException&) {
                std::cout << "Employee not found.\n";
            }
        } 
        else if (position == "Customer") {
            std::string id = readString("Enter your id (or 'New' to register): ");
            
            if (id == "New" || id == "new") {
                std::string name = readString("Enter your name: ");
                std::string pass = readString("Enter your password (min 8 chars): ");
                while (pass.length() < 8) {
                    std::cout << "Password must be at least 8 characters long.\n";
                    pass = readString("Enter your password: ");
                }
                Customer newCust(name, pass);
                mgr.addCustomer(newCust);
                std::cout << "Registered successfully. Your ID is: " << newCust.getId() << "\n";
                continue;
            }
            
            try {
                auto cust = mgr.getCustomer(id);
                std::string pass = readString("Enter your password: ");
                int counter = 0;
                while (!cust->checkpass(pass)) {
                    std::cout << "Incorrect password.\n";
                    counter++;
                    if (counter >= 3) {
                        std::cout << "Too many incorrect attempts. Returning to main menu.\n";
                        break;
                    }
                    pass = readString("Enter your password again: ");
                }
                if (cust->checkpass(pass)) {
                    customerMenu(mgr, rental, id);
                }
            } catch (const NotFoundException&) {
                std::cout << "Customer not found.\n";
            }
        } 
        else {
            std::cout << "Invalid position.\n";
        }
    }

    mgr.saveData();
    std::cout << "Thank you for using the car rental system.\n";
    return 0;
}
