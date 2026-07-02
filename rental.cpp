#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

const std::string AVAILABLE_OWNER = "M";
class Car {
private:
    int id;
    std::string make;
    std::string model;
    std::string color;
    std::string Condition;
    
public:
    std::string ownerId;
    int year;
    static int nextId;
    float dailyRate;
    int dueDate;
    
    Car()
        : id(-1), ownerId(""), year(0), dailyRate(0.0), dueDate(0){}

    Car(const std::string& make, const std::string& model, const std::string& color,
        const std::string& Condition, int year, double dailyRate){
        id = nextId++;
        this->make = make;
        this->model = model;
        this->color = color;
        this->Condition = Condition;
        this->ownerId = AVAILABLE_OWNER;
        this->year = year;
        this->dailyRate = dailyRate;
        this->dueDate = 0;
        }

    void display() const {
        std::cout << "ID: " << id << ", Make: " << make << ", Model: " << model
             << ", Color: " << color << ", Condition: " << Condition << ", Owner ID: "
             << ownerId << ", Year: " << year << ", Daily Rate: $" << std::fixed
             << std::setprecision(2) << dailyRate << ", Due Date: " << dueDate
             << std::endl;
    }
    void RentRequest(const std::string& id, int duedate){
        ownerId=id;
        dueDate=duedate;
    }
    int getId() const {
        return id;
    }
    const std::string& getMake() const { return make; }
    const std::string& getModel() const { return model; }
    void setDueDate(int date){
        dueDate=date;
    }
    int getDueDate() const {
        return dueDate;
    }
    bool isAvailable(int date) const {
        return (ownerId == AVAILABLE_OWNER || dueDate < date);
    }
    friend std::ostream& operator<<(std::ostream& out, const Car& car);
    friend std::istream& operator>>(std::istream& in, Car& car);
};
int Car::nextId = 0; // Initialize the static variable
std::ostream& operator<<(std::ostream& out, const Car& car) {
    out << car.id << ' ' << car.make << ' ' << car.model << ' ' << car.color
        << ' ' << car.Condition << ' ' << car.ownerId << ' ' << car.year << ' '
        << car.dailyRate << ' ' << car.dueDate << ' ' << '\n';
    return out;
}
std::istream& operator>>(std::istream& in, Car& car) {
    in >> car.id >> car.make >> car.model >> car.color >> car.Condition >>
        car.ownerId >> car.year >> car.dailyRate >> car.dueDate;
    return in;
}
std::vector<Car> cars;
void updateCar(Car car);
Car getCar(int id){
    if (cars.empty()){
        std::cout<<"No cars found.\n";
        Car car;
        return car;
    }
    for (const auto& i:cars){
        if (i.getId()==id){
            return i;
        }
    }
    Car car;
    return car;
}
class User {
protected:
std::string id;
std::string name;
std::string password;
public:
    std::string position;
    int record;
    int Payable;
    int numberofCars; 
    std::vector<int> cars;
    void display() const {
        std::cout << "ID: " << id << ", Name: " << name << ", Position: " << position
             << ", Record: " << record << ", Payable: " << Payable << ", Car Count: " << numberofCars << std::endl<<"Cars: ";
        if (numberofCars>0) for (const auto& car : cars) {
            if (car!=-1) std::cout<<car<<" ";
        }
        std::cout<<std::endl;
    }
    const std::string& getId() const {
        return id;
    }
    void setpass (const std::string& pass){
        password=pass;
    }
    void clear_due(){
        Payable=0;
    }
    bool checkpass(const std::string& pass) const {
        return pass==password;
    }
    void setname (const std::string& name){
        this->name=name;
    }
    const std::string& getpass() const {
        return password;
    }
    const std::string& getname() const {
        return name;
    }
    void addCar(int carId) {
        cars.insert(cars.begin(), carId);
        numberofCars++;
    }
    void showcars() const {
        if (cars.empty()){
            std::cout<<"You don't own a car.\n";
        }
        for (const auto& i:cars){
            if (i!=-1){
                Car car=getCar(i);
                car.display();
            }
        }
    }
    void removeCar(int carId) {
        for (auto it = cars.begin(); it != cars.end(); ++it) {
            if (*it == carId) {
                cars.erase(it);
                numberofCars--;
                return;
            }
        }
    }   
};
class Employee: public User{
    public:
    static int employeeNextId;
    Employee(const std::string& namenew, const std::string& passwordnew){
        name=namenew;
        password=passwordnew;
        position="Employee";
        id="E"+std::to_string(employeeNextId++);
        if (namenew==""||passwordnew==""){
            employeeNextId--;
        }
        cars.push_back(-1);
        numberofCars=0;
        record=0;
        Payable=0;
    }
    bool canRent() const {
        if (record<=-100) {
            std::cout<<"Bad record.\n";
            return false;
        }
        int limit;
        if (record>=50){
            limit=10;
        }
        else if (record>=-20) limit=(record+40)/10;
        else limit=1;
        if (this->numberofCars>=limit) {
            std::cout<<"Maximum cars rented.\n";
            return false;
        }
        if (this->Payable>=150000) {
            std::cout<<"Payable limit reached.\n";
            return false;
        }
        return true;
    }
    void rentCar(int carId, int startdate,int duedate) {
        Car car = getCar(carId);
        if (car.ownerId.empty()) {
            std::cout << "Car not found.\n";
            return;
        }
        if (!car.isAvailable(startdate)) {
            std::cout << "Car is not available.\n";
            return;
        }
        if (!this->canRent()) {
            std::cout << "Employee cannot rent more cars.\n";
            return;
        }
        car.RentRequest(id,duedate);
        this->addCar(carId);
        this->Payable += (car.dailyRate)*(duedate-startdate)*0.85;
        updateCar(car);
        std::cout << "Car rented successfully.\n";
    }
    void returnCar(int carId, int current_date) {
        Car car = getCar(carId);
        if (car.ownerId.empty()) {
            std::cout << "Car not found.\n";
            return;
        }
        if (car.ownerId != this->id) {
            std::cout << "Car is not rented by you.\n";
            return;
        }
        if (car.getDueDate() < current_date) {
            this->Payable += (car.dailyRate)*(current_date-car.dueDate)*1.1;
            this->record -= 10 * (current_date - car.dueDate);
        }
        else if (car.getDueDate() > current_date){
            this -> record += 10*(car.dueDate - current_date);
        }
        car.ownerId = AVAILABLE_OWNER;
        updateCar(car);
        this-> removeCar(carId);
        std::cout << "Car returned successfully.\n";
    }
    friend std::ostream& operator<<(std::ostream& out, const Employee& emp);
    friend std::istream& operator>>(std::istream& in, Employee& emp);
};
std::ostream& operator<<(std::ostream& out, const Employee& emp) {
    out << emp.id << ' ' << emp.name << ' ' <<emp.password<<' '<< emp.position << ' ' << emp.record << ' ' << emp.Payable << ' '<<emp.numberofCars<<' ';
    for (const auto& car : emp.cars) {
        out << car << ' ';
    }
    out << "\n";
    return out;
}
std::istream& operator>>(std::istream& in, Employee& emp) {
    in >> emp.id >> emp.name>>emp.password>>emp.position>> emp.record >> emp.Payable>>emp.numberofCars;
    int carId;
    emp.cars.clear();
    if (emp.numberofCars==0){
        emp.cars.push_back(-1);
        in>>carId;
    }
    else{
        while (true) {
        in >> carId;
        if (carId == -1) {
            break; // End of car IDs
        }
        emp.cars.push_back(carId);
        }
        emp.cars.push_back(-1);
    }
    return in;
}
int Employee::employeeNextId=0;
class Customer: public User{
    public:
    static int customerNextId;
    static int avgcustomerRating;
    Customer(const std::string& namenew, const std::string& passwordnew){
        name=namenew;
        password=passwordnew;
        position="Customer";
        id="C"+std::to_string(customerNextId++);
        if (namenew==""||passwordnew==""){
            customerNextId--;
        }
        cars.push_back(-1);
        numberofCars=0;
        record=avgcustomerRating;
        Payable=0;
    }
    bool canRent() const {
        if (record<=-50) {
            std::cout<<"Bad record.\n";
            return false;
        }
        int limit;
        if (record>=40){
            limit=7;
        }
        else if (record>=-20) limit=(record+40)/10;
        else limit=1;
        if (this->numberofCars>=limit) {
            std::cout<<"Maximum cars rented.\n";
            return false;
        }
        return true;
    }
    void rentCar(int carId, int startdate,int duedate) {
        Car car = getCar(carId);
        if (car.ownerId.empty()) {
            std::cout << "Car not found.\n";
            return;
        }
        
        if (!car.isAvailable(startdate)) {
            std::cout << "Car is not available.\n";
            return;
        }
        if (!canRent()) {
            std::cout << "Customer cannot rent more cars.\n";
            return;
        }
        car.RentRequest(id,duedate);
        updateCar(car);
        this->addCar(carId);
        this->Payable += (car.dailyRate)*(duedate-startdate);
        std::cout << "Car rented successfully.\n";
    }
    void returnCar(int carId, int current_date) {
        Car car = getCar(carId);
        if (car.ownerId.empty()) {
            std::cout << "Car not found.\n";
            return;
        }
        if (car.ownerId != this->id) {
            std::cout << "Car is not rented by you.\n";
            return;
        }
        if (car.dueDate < current_date) {
            this->Payable += (car.dailyRate)*(current_date-car.dueDate)*1.25;
            this->record -= 10 * (current_date - car.dueDate);
        }
        else if (car.dueDate > current_date){
            this->record += 10*(car.dueDate - current_date);
        }
        car.ownerId = AVAILABLE_OWNER;
        car.dueDate = 0;
        updateCar(car);
        this-> removeCar(carId);
        std::cout << "Car returned successfully.\n";
    }
    friend std::ostream& operator<<(std::ostream& out, const Customer& cst);
    friend std::istream& operator>>(std::istream& in, Customer& cst);
};
int Customer::customerNextId=0;
int Customer::avgcustomerRating=0;
std::ostream& operator<<(std::ostream& out, const Customer& cst) {
    out << cst.id << ' ' << cst.name << ' ' << cst.password<<' '<<cst.position << ' ' << cst.record << ' ' << cst.Payable << ' '<<cst.numberofCars<<' ';
    for (const auto& car : cst.cars) {
        out << car << ' ';
    }
    out << "\n";
    return out;
}
std::istream& operator>>(std::istream& in, Customer& cst) {
    in >> cst.id >> cst.name>>cst.password>>cst.position>> cst.record >> cst.Payable>>cst.numberofCars;
    int carId;
    cst.cars.clear();
    if (cst.numberofCars==0){
        cst.cars.push_back(-1);
        in>>carId;
    }
    else{
        while (true) {
        in >> carId;
        if (carId == -1) {
            break; // End of car IDs
        }
        cst.cars.push_back(carId);
        }
        cst.cars.push_back(-1);
    }
    return in;
}
std::vector<Employee> employees;
std::vector<Customer> customers;
Employee getEmployee(const std::string& id){
        for (auto& i:employees){
            if (i.getId()==id){
                return i;
            }
        }
        Employee emp("","");
        return emp;
    }
Customer getCustomer(const std::string& id){
        for (auto& i:customers){
            if (i.getId()==id){
                return i;
            }
        }
        Customer cust("","");
        return cust;
    }
class Manager {
public:
    void addCar(const Car& car) {
        cars.push_back(car);
    }
    void deleteCar (int carId){
        for (auto it = cars.begin(); it != cars.end(); ++it) {
            if (it->getId() == carId) {
                cars.erase(it);
                std::cout << "Car deleted successfully.\n";
                return;
            }
        }
        std::cout << "Car not found.\n";
    }
    void displayAllCars() const {
        if (cars.empty()){
            std::cout<<"No cars found.\n";
        }
        for (const auto& i:cars){
            i.display();
        }
    }
    void displayAllEmployees() const {
        if (employees.empty()){
            std::cout<<"No employees found.\n";
        }
        for (const auto& i:employees){
            i.display();
        }
    }
    void displayAllCustomers() const {
        if (customers.empty()){
            std::cout<<"No customers found.\n";
        }
        for (const auto& i:customers){
            i.display();
        }
    }
    void deleteCustomer(const std::string& id){
        for (auto it = customers.begin(); it != customers.end(); ++it) {
            if (it->getId() == id) {
                Customer cust = *it;
                for (auto& car : cust.cars) {
                    Car carobj = getCar(car);
                    carobj.RentRequest(AVAILABLE_OWNER, 0);
                    updateCar(carobj);
                }
                customers.erase(it);
                return;
            }
        }
        std::cout << "Customer not found.\n";
    }
    void addEmployee(const Employee& emp) {
        employees.push_back(emp);
    }
    void addCustomer(const Customer& cust) {
        customers.push_back(cust);
    }
    void deleteEmployee(const std::string& id){
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getId() == id) {
                employees.erase(it);
                return;
            }
        }
        std::cout << "Employee not found.\n";
    }
    friend class Customer;
    friend class Employee;
};
void saveCarsToFile(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& car : cars) {
            outFile << car;
        }
        outFile.close();
    }
void loadCarsFromFile(const std::string& fileName) {
        std::ifstream inFile(fileName);
        if (!inFile) {
            std::cerr << "Error opening file for reading.\n";
            return;
        }
        Car car;
        while (inFile >> car) {
            cars.push_back(car);
            Car::nextId = std::max(Car::nextId, car.getId() + 1);
        }
        inFile.close();
    }
void saveEmployeesToFile(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& u : employees) {
                outFile << u;
        }
        outFile.close();
    }
void saveCustomersToFile(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ios::trunc);
        if (!outFile) {
            std::cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& u : customers) {
                outFile << u;
        }
        outFile.close();
    }
void loadEmployeesFromFile(const std::string& fileName) {
        std::ifstream inFile(fileName);

        if (!inFile) {
            std::cerr << "Error opening file for reading.\n";
            return;
        }
        Employee emp = Employee("","");
        while (inFile >> emp) {
            employees.push_back(emp);
            Employee::employeeNextId = std::max(Employee::employeeNextId, std::stoi(emp.getId().substr(1)) + 1);
        }
        inFile.close();
    }
void loadCustomersFromFile(const std::string& fileName) {
        std::ifstream inFile(fileName);
        if (!inFile) {
            std::cerr << "Error opening file for reading.\n";
            return;
        }
        Customer cust("","");
        while (inFile >> cust) {
            customers.push_back(cust);
            Customer::customerNextId = std::max(Customer::customerNextId, std::stoi(cust.getId().substr(1)) + 1);
            Customer::avgcustomerRating+=cust.record;
        }
        if (!customers.empty()) {
            Customer::avgcustomerRating/=customers.size();
        }

        inFile.close();
    }
void displayManagerchoices(){
    std::cout<<"Enter your choice:\n";
    std::cout<<"0. Change Password\n";
    std::cout<<"1. Add a car\n";
    std::cout<<"2. Delete a car\n";
    std::cout<<"3. Display all cars\n";
    std::cout<<"4. Add an employee\n";
    std::cout<<"5. Delete an employee\n";
    std::cout<<"6. Display all employees\n";
    std::cout<<"7. Add a customer\n";
    std::cout<<"8. Delete a customer\n";
    std::cout<<"9. Display all customers\n";
    std::cout<<"10. Update an Employee\n";
    std::cout<<"11. Update a Customer\n";
    std::cout<<"12. Update a Car\n";
    std::cout<<"13. Exit\n";
}
void displayEmployeeChoices(){
    std::cout<<"Enter your choice:\n";
    std::cout<<"1. Rent a car\n";
    std::cout<<"2. Return a car\n";
    std::cout<<"3. Display all Available cars\n";
    std::cout<<"4. Display all your cars\n";
    std::cout<<"5. Clear due\n";
    std::cout<<"7. Exit\n";
}
void displayCustomerChoices(){
    std::cout<<"Enter your choice:\n";
    std::cout<<"1. Rent a car\n";
    std::cout<<"2. Return a car\n";
    std::cout<<"3. Display all cars\n";
    std::cout<<"4. Display all your cars\n";
    std::cout<<"5. Clear due\n";
    std::cout<<"7. Exit\n";
}
void updateCar(Car car){
    for (auto& i:cars){
        if (i.getId()==car.getId()){
            i=car;
            return;
        }
    }
}
void updateEmployee(Employee emp){
    for (auto& i:employees){
        if (i.getId()==emp.getId()){
            i=emp;
            return;
        }
    }
}
void updateCustomer(Customer cust){
    for (auto& i:customers){
        if (i.getId()==cust.getId()){
            i=cust;
            return;
        }
    }
}
// write a function to check for valid date between 2020 to 2025
bool isValidDate(int d){
    if (d<20200000||d>20250000){
        return false;
    }
    else if (d%100>31||d%100==0){
        return false;
    }
    else if (d%10000-d%100>1200||d%10000-d%100==0) {
        return false;
    }
    return true; 
}
void showAvailable(int date){
    int counter=0;
    for (const auto& i:cars){
        if (i.isAvailable(date)){
            i.display();
            counter++;
        }
    }
    if (counter==0){
        std::cout<<"No cars available at the given date. Sorry for your inconvenience!\n";
    }
    std::cout<<std::endl;
}
int main() {
    Manager database;
    std::string Manager_pass="Mrental001";
    loadCarsFromFile("cars.txt");
    loadEmployeesFromFile("employees.txt");
    loadCustomersFromFile("customers.txt");
    std::cout<<"Welcome to the car rental system.\n";
    std::cout<<"Please enter your position (Manager, Employee, Customer): ";
    std::string position;
    std::cin>>position;
    if (position=="Manager"){
        std::cout<<"Please enter the password: ";
        std::string pass;
        std::cin>>pass;
        int counter=0;
        while (pass!=Manager_pass){
            std::cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                std::cout<<"Too many incorrect attempts.\n";
                return 0;
            }
            std::cout<<"Please enter the password again: ";
            std::cin>>pass;
        }
        std::cout<<"Welcome, Manager.\n";
        int choice;
        displayManagerchoices();
        std::cin>>choice;
        while (choice!=13){
            if (choice==0){
                std::cout<<"Enter the new password: ";
                std::cin>>Manager_pass;
                while (Manager_pass.length()<8){
                    std::cout<<"Password must be at least 8 characters long.\n";
                    std::cout<<"Enter the new password: ";
                    std::cin>>Manager_pass;
                }
                std::cout<<"Password changed successfully.\n";
            }
            if (choice==1){
                std::string make,model,color,Condition;
                int year;
                double dailyRate;
                std::cout<<"Enter the make of the car: ";
                std::cin>>make;
                std::cout<<"Enter the model of the car: ";
                std::cin>>model;
                std::cout<<"Enter the color of the car: ";
                std::cin>>color;
                std::cout<<"Enter the Condition of the car: (G: Good, B: Bad, D: Doable)";
                std::cin>>Condition;
                std::cout<<"Enter the year of the car: ";
                std::cin>>year;
                std::cout<<"Enter the daily rate of the car: ";
                std::cin>>dailyRate;
                Car car(make,model,color,Condition,year,dailyRate);
                database.addCar(car);
                std::cout << "Car added successfully.\n";
                
            }
            else if (choice==2){
                int id;
                std::cout<<"Enter the id of the car: ";
                std::cin>>id;
                database.deleteCar(id);
            }
            else if (choice==3){
                database.displayAllCars();
            }
            else if (choice==4){
                std::string name,pass;
                std::cout<<"Enter the name of the employee: ";
                std::cin>>name;
                std::cout<<"Enter the password of the employee: ";
                std::cin>>pass;
                while (pass.length()<8){
                    std::cout<<"Password must be at least 8 characters long.\n";
                    std::cout<<"Enter the password of the employee: ";
                    std::cin>>pass;
                }
                Employee emp(name,pass);
                database.addEmployee(emp);
                std::cout << "Employee added successfully.\n";
            }
            else if (choice==5){
                std::string id;
                std::cout<<"Enter the id of the employee: ";
                std::cin>>id;
                database.deleteEmployee(id);
            }
            else if (choice==6){
                database.displayAllEmployees();
            }
            else if (choice==7){
                std::string name,pass;
                std::cout<<"Enter the name of the customer: ";
                std::cin>>name;
                std::cout<<"Enter the password of the customer: ";
                std::cin>>pass;
                while (pass.length()<8){
                    std::cout<<"Password must be at least 8 characters long.\n";
                    std::cout<<"Enter the password of the customer: ";
                    std::cin>>pass;
                }
                Customer cust(name,pass);
                database.addCustomer(cust);
                std::cout << "Customer added successfully.\n";
            }
            else if (choice==8){
                std::string id;
                std::cout<<"Enter the id of the customer: ";
                std::cin>>id;
                database.deleteCustomer(id);
            }
            else if (choice==9){
                database.displayAllCustomers();     
            }
            else if (choice==10){
                std::string id;
                std::cout<<"Enter the id of the employee: ";
                std::cin>>id;
                Employee emp=getEmployee(id);
                if (emp.getname().empty() && emp.getpass().empty()){
                    std::cout<<"Employee not found.\n";
                }
                else {
                emp.display();
                std::cout<<"Do you want to update the record? (Y/N): ";
                char ch;
                std::cin>>ch;
                if (ch=='Y'){
                    std::cout<<"Enter the new record: ";
                    int record;
                    std::cin>>record;
                    emp.record=record;
                    emp.display();
                }
                updateEmployee(emp);
                std::cout<<"Do you want to update the payable? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    std::cout<<"Enter the new payable: ";
                    int payable;
                    std::cin>>payable;
                    emp.Payable=payable;
                    emp.display();
                }
                updateEmployee(emp);
                std::cout<<"Do you want to remove a car? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    int carId;
                    std::cout<<"Enter the id of the car: ";
                    std::cin>>carId;
                    emp.removeCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        std::cout<<"Car not found.\n";
                    }
                    else car.RentRequest(AVAILABLE_OWNER, 0);
                    emp.display();
                }
                updateEmployee(emp);
                std::cout<<"Do you want to add a car? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    int carId;
                    std::cout<<"Enter the id of the car: ";
                    std::cin>>carId;
                    emp.addCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        std::cout<<"Car not found.\n";
                    }
                    else if (car.ownerId!=AVAILABLE_OWNER){
                        std::cout<<"Car already rented.\n";
                    }
                    else {
                        std::cout<<"Enter the due date: ";
                        int duedate;
                        std::cin>>duedate;
                        while (isValidDate(duedate)==false){
                            std::cout<<"Invalid date.\n";
                            std::cin>>duedate;
                        }
                        car.RentRequest(emp.getId(),duedate);
                    }
                    emp.display();
                }
                updateEmployee(emp);
                }
            }
            else if (choice==11){
                std::string id;
                std::cout<<"Enter the id of the customer: ";
                std::cin>>id;
                Customer cust=getCustomer(id);
                if (cust.getname().empty() && cust.getpass().empty()){
                    std::cout<<"Customer not found.\n";
                }
                else {
                cust.display();
                std::cout<<"Do you want to update the record? (Y/N): ";
                char ch;
                std::cin>>ch;
                if (ch=='Y'){
                    std::cout<<"Enter the new record: ";
                    int record;
                    std::cin>>record;
                    cust.record=record;
                    cust.display();
                }
                updateCustomer(cust);
                std::cout<<"Do you want to update the payable? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    std::cout<<"Enter the new payable: ";
                    int payable;
                    std::cin>>payable;
                    cust.Payable=payable;
                    cust.display();
                }
                updateCustomer(cust);
                std::cout<<"Do you want to remove a car? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    int carId;
                    std::cout<<"Enter the id of the car: ";
                    std::cin>>carId;
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        std::cout<<"Car not found.\n";
                    }
                    else if (car.ownerId!=cust.getId()){
                        std::cout<<"Car not rented by the customer.\n";
                    }
                    else {
                    cust.removeCar(carId);
                    car.RentRequest(AVAILABLE_OWNER, 0);
                    }
                    cust.display();
                }
                updateCustomer(cust);
                std::cout<<"Do you want to add a car? (Y/N): ";
                std::cin>>ch;
                if (ch=='Y'){
                    int carId;
                    std::cout<<"Enter the id of the car: ";
                    std::cin>>carId;
                    cust.addCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        std::cout<<"Car not found.\n";
                    }
                    else {
                        if (car.ownerId!=AVAILABLE_OWNER){
                            std::cout<<"Car already rented.\n";
                        }
                        else {
                        std::cout<<"Enter the due date: ";
                            int duedate;
                            std::cin>>duedate;
                            while (isValidDate(duedate)==false){
                                std::cout<<"Invalid date.\n";
                                std::cin>>duedate;
                            }
                            car.RentRequest(cust.getId(),duedate);
                        }
                    }
                    cust.display();
                }
                updateCustomer(cust);
                }
            }
            else if (choice==12){
                int id;
                std::cout<<"Enter the id of the car: ";
                std::cin>>id;
                Car car=getCar(id);
                if (car.getId()==-1){
                    std::cout<<"Car not found.\n";
                    return 0;
                }
                car.display();
                std::cout<<"Do you want to update the due date? (Y/N): ";
                char ch;
                std::cin>>ch;
                if (ch=='Y'){
                    std::cout<<"Enter the new due date: ";
                    int duedate;
                    std::cin>>duedate;
                    car.setDueDate(duedate);
                    car.display();
                }
                updateCar(car);
                std::cout << "Car updated successfully.\n";
                std::cout << "Do you want to update the price? (Y/N): ";
                std::cin >> ch;
                if (ch == 'Y') {
                    std::cout << "Enter the new daily rate: ";
                    double dailyRate;
                    std::cin >> dailyRate;
                    car.dailyRate = dailyRate;
                    car.display();
                }
                updateCar(car);

            }
            else std::cout<<"Invalid choice.\n";
            displayManagerchoices();
            std::cin>>choice;
        }
    }
    else if (position=="Employee"){
        std::cout<<"Enter your id: ";
        std::string id;
        std::cin>>id;
        Employee emp=getEmployee(id);
        if (emp.getname().empty() && emp.getpass().empty()){
            std::cout<<"Employee not found.\n";
            return 0;
        }
        std::cout<<"Enter your password: ";
        std::string pass;
        std::cin>>pass;
        int counter=0;
        while (emp.checkpass(pass)==false){
            std::cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                std::cout<<"Too many incorrect attempts.\n";
                return 0;
            }
            std::cout<<"Please enter your password again:\n";
            std::cin>>pass;
        }
        std::cout<<"Welcome, " << emp.getname() <<".\n";
        emp.display();
        int choice;
        displayEmployeeChoices();
        std::cin>>choice;
        while (choice!=7){
            if (choice==1){
                int carId,startdate,duedate;
                std::cout<<"Enter the id of the car: ";
                std::cin>>carId;
                std::cout<<"Enter the start date (in YYYYMMDD format):";
                std::cin>>startdate;
                while (isValidDate(startdate)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>startdate;
                }
                std::cout<<"Enter the due date (YYYYMMDD format):";
                std::cin>>duedate;
                while (duedate<startdate||isValidDate(duedate)==false){
                    std::cout<<"Invalid date. Enter correct date\n";
                    std::cin>>duedate;
                }
                emp.rentCar(carId,startdate,duedate);
            }
            else if (choice==2){
                int carId,current_date;
                std::cout<<"Enter the id of the car: ";
                std::cin>>carId;
                std::cout<<"Enter the current date(YYYYMMDD format): ";
                std::cin>>current_date;
                while (isValidDate(current_date)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>current_date;
                }
                emp.returnCar(carId,current_date);
            }
            else if (choice==3){
                int date;
                std::cout<<"Enter the date (YYYYMMDD format): ";
                std::cin>>date;
                while (isValidDate(date)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>date;
                }
                showAvailable(date);
            }
            else if (choice==4){
                emp.showcars();
            }
            else if (choice==5){
                emp.clear_due();
                emp.display();
            }
            else{
                std::cout<<"Invalid choice.\n";
            }
            displayEmployeeChoices();
            emp.display();
            std::cin>>choice;
            updateEmployee(emp);
        }
        
    }
    else if (position=="Customer"){
        std::cout<<"Enter your id: ";
        std::string id;
        std::cin>>id;
        Customer cust=getCustomer(id);
        if (cust.getname().empty() && cust.getpass().empty()){
            std::cout<<"Customer not found.\n";
            std::cout << "Do you want to register as a new customer? (Y/N): ";
            char ch;
            std::cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                std::cout << "Enter your name: ";
                std::string name;
                std::cin >> name;
                std::cout << "Enter your password: ";
                std::string pass;
                std::cin >> pass;
                while (pass.length() < 8) {
                    std::cout << "Password must be at least 8 characters long.\n";
                    std::cout << "Enter your password: ";
                    std::cin >> pass;
                }
                Customer custnew(name, pass);
                database.addCustomer(custnew);
                std::cout << "Customer added successfully.\n";
                saveCustomersToFile("customers.txt");
                cust = custnew;
            }
            else return 0;
        }
        std::cout<<"Enter your password: ";
        std::string pass;
        std::cin>>pass;
        int counter=0;
        while (cust.checkpass(pass)==false){
            std::cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                std::cout<<"Too many incorrect attempts.\n";
                return 0;
            }
            std::cout<<"Please enter your password again:\n";
            std::cin>>pass;
        }
        std::cout<<"Welcome, "<<cust.getname()<<".\n";
        cust.display();
        int choice;
        displayCustomerChoices();
        std::cin>>choice;
        while (choice!=7){
            if (choice==1){
                int carId,startdate,duedate;
                std::cout<<"Enter the id of the car: ";
                std::cin>>carId;
                std::cout<<"Enter the start date (in YYYYMMDD format):";
                std::cin>>startdate;
                while (isValidDate(startdate)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>startdate;
                }
                std::cout<<"Enter the due date (YYYYMMDD format):";
                std::cin>>duedate;
                while (duedate<startdate||isValidDate(duedate)==false){
                    std::cout<<"Invalid date. Enter correct date\n";
                    std::cin>>duedate;
                }
                cust.rentCar(carId,startdate,duedate);
            }
            else if (choice==2){
                int carId,current_date;
                std::cout<<"Enter the id of the car: ";
                std::cin>>carId;
                std::cout<<"Enter the current date: ";
                std::cin>>current_date;
                while (isValidDate(current_date)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>current_date;
                }
                cust.returnCar(carId,current_date);
            }
            else if (choice==3){
                int date;
                std::cout<<"Enter the date (YYYYMMDD format): ";
                std::cin>>date;
                while (isValidDate(date)==false){
                    std::cout<<"Invalid date.\n";
                    std::cin>>date;
                }
                showAvailable(date);
            }
            else if (choice==4){
                cust.showcars();
            }
            else if (choice==5){
                cust.clear_due();
                cust.display();
            }
            else{
                std::cout<<"Invalid choice.\n";
            }
            cust.display();
            displayCustomerChoices();
            std::cin>>choice;
            updateCustomer(cust);
        }
    }
    else std::cout<<"Invalid position.\n";
    saveCarsToFile("cars.txt");
    saveEmployeesToFile("employees.txt");
    saveCustomersToFile("customers.txt");
    std::cout<<"Thank you for using the car rental system.\n";
    return 0;
}
