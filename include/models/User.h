#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <iostream>

class User {
protected:
    std::string name;
    std::string password;
    std::string position;
    std::string id;

public:
    int record;
    int Payable;
    int numberofCars;
    std::vector<int> cars;

    User();
    
    std::string getname() const;
    std::string getpass() const;
    std::string getPosition() const;
    std::string getId() const;
    
    bool checkpass(const std::string& pass) const;
    void addCar(int carId);
    void removeCar(int carId);
    void showcars() const;
    void clear_due();
    void display() const;

    // A helper method that will be useful for saving/loading
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);
};

#endif // USER_H
