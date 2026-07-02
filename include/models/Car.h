#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>

const std::string AVAILABLE_OWNER = "M";

class Car {
private:
    int id;
    std::string make;
    std::string model;
    std::string color;
    std::string condition;
    std::string ownerId;
    int year;
    double dailyRate;
    int dueDate;

public:
    static int nextId;

    Car();
    Car(std::string make, std::string model, std::string color, std::string condition, int year, double dailyRate);

    void display() const;
    void RentRequest(std::string u_id, int dueDate);
    int getId() const;
    void setDueDate(int dueDate);
    int getDueDate() const;
    bool isAvailable(int date) const;
    std::string getOwnerId() const;
    double getDailyRate() const;

    friend std::ostream& operator<<(std::ostream& os, const Car& car);
    friend std::istream& operator>>(std::istream& is, Car& car);
};

#endif // CAR_H
