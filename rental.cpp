#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
class Car {
private: 
    int id;
    string make;
    string model;
    string color;
    string Condition;
    
public:
    string ownerId;
    int year;
    static int nextId;
    float dailyRate;
    int dueDate;
    
    Car()
        : id(-1), ownerId(""), year(0), dailyRate(0.0), dueDate(0){}

    Car(const string& make, const string& model, const string& color,
        const string& Condition, int year, double dailyRate){
        id = nextId++;
        this->make = make;
        this->model = model;
        this->color = color;
        this->Condition = Condition;
        this->ownerId = "M";
        this->year = year;
        this->dailyRate = dailyRate;
        this->dueDate = 00000000;
        }

    void display() const {
        cout << "ID: " << id << ", Make: " << make << ", Model: " << model
             << ", Color: " << color << ", Condition: " << Condition << ", Owner ID: "
             << ownerId << ", Year: " << year << ", Daily Rate: $" << fixed
             << setprecision(2) << dailyRate << ", Due Date: " << dueDate
             << endl;
    }
    void RentRequest(string id,int duedate){
        ownerId=id;
        dueDate=duedate;
    }
    int getId() const {
        return id;
    }
    void setDueDate(int date){
        dueDate=date;
    }
    int getDueDate(){
        return dueDate;
    }
    bool isAvailable(int date) {
        if (ownerId == "M" || dueDate < date) {
            return true;
        } 
        else {
            return false;
        }
    }
    friend ostream& operator<<(ostream& out, const Car& car);
    friend istream& operator>>(istream& in, Car& car);
};
int Car::nextId = 0; // Initialize the static variable
ostream& operator<<(ostream& out, const Car& car) {
    out << car.id << ' ' << car.make << ' ' << car.model << ' ' << car.color
        << ' ' << car.Condition << ' ' << car.ownerId << ' ' << car.year << ' '
        << car.dailyRate << ' ' << car.dueDate << ' ' << '\n';
    return out;
}
istream& operator>>(istream& in, Car& car) {
    in >> car.id >> car.make >> car.model >> car.color >> car.Condition >>
        car.ownerId >> car.year >> car.dailyRate >> car.dueDate;
    return in;
}
vector<Car> cars;
void updateCar(Car car);
Car getCar(int id){
    if (cars.size()==0){
        cout<<"No cars found.\n";
        Car car;
        return car;
    }
    for (auto i:cars){
        if (i.getId()==id){
            return i;
        }
    }
    Car car;
    return car;
}
class User {
protected:
string id;
string name;
string password;
public:
    string position;
    int record;
    int Payable;
    int numberofCars; 
    vector<int> cars;
    void display() {
        cout << "ID: " << id << ", Name: " << name << ", Position: " << position
             << ", Record: " << record << ", Payable: " << Payable << ", Car Count: " << numberofCars << endl<<"Cars: ";
        if (numberofCars>0) for (auto& car : cars) {
            if (car!=-1) cout<<car<<" ";
        }
        cout<<endl;
    }
    string getId() const {
        return id;
    }
    void setpass (string pass){
        password=pass;
    }
    void clear_due(){
        Payable=0;
    }
    bool checkpass(string pass){
        return pass==password;
    }
    void setname (string name){
        this->name=name;
    }
    string getpass(){
        return password;
    }
    string getname(){
        return name;
    }
    void addCar(int carId) {
        cars.insert(cars.begin(), carId);
        numberofCars++;
    }
    void showcars(){
        if (cars.size()==0){
            cout<<"You don't own a car.\n";
        }
        for (auto i:cars){
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
class employee: public User{
    public:
    static int employeeNextId;
    employee(const string& namenew, const string& passwordnew){
        name=namenew;
        password=passwordnew;
        position="Employee";
        id="E"+to_string(employeeNextId++);
        if (namenew==""||passwordnew==""){
            employeeNextId--;
        }
        cars.push_back(-1);
        numberofCars=0;
        record=0;
        Payable=0;
    }
    bool canRent() {
        if (record<=-100) {
            cout<<"Bad record.\n";
            return false;
        }
        else {
            int limit;
            if (record>=50){
                limit=10;
            }
            else if (record>=-20) limit=(record+40)/10;
            else limit=1;
            if (this->numberofCars>=limit) cout <<"Maximum cars rented.\n";
                return false;
        }
        if (this->Payable>=150000) {
            cout<<"Payable limit reached.\n";
            return false;
        }
        return true;
    }
    void rentCar(int carId, int startdate,int duedate) {
        Car car = getCar(carId);
        if (car.ownerId=="") {
            cout << "Car not found.\n";
            return;
        }
        if (!car.isAvailable(startdate)) {
            cout << "Car is not available.\n";
            return;
        }
        if (!this->canRent()) {
            cout << "Employee cannot rent more cars.\n";
            return;
        }
        car.RentRequest(id,duedate);
        this->addCar(carId);
        this->Payable += (car.dailyRate)*(duedate-startdate)*0.85;
        updateCar(car);
        cout << "Car rented successfully.\n";
    }
    void returnCar(int carId, int current_date) {
        Car car = getCar(carId);
        if (car.ownerId=="") {
            cout << "Car not found.\n";
            return;
        }
        if (car.ownerId != this->id) {
            cout << "Car is not rented by you.\n";
            return;
        }
        if (car.getDueDate() < current_date) {
            this->Payable += (car.dailyRate)*(current_date-car.dueDate)*1.1;
            this->record -= 10 * (current_date - car.dueDate);
        }
        else if (car.getDueDate() > current_date){
            this -> record += 10*(car.dueDate - current_date);
        }
        car.ownerId = "M";
        updateCar(car);
        this-> removeCar(carId);
        cout << "Car returned successfully.\n";
    }
    friend ostream& operator<<(ostream& out, const employee& emp);
    friend istream& operator>>(istream& in, employee& emp);
};
ostream& operator<<(ostream& out, const employee& emp) {
    out << emp.id << ' ' << emp.name << ' ' <<emp.password<<' '<< emp.position << ' ' << emp.record << ' ' << emp.Payable << ' '<<emp.numberofCars<<' ';
    for (const auto& car : emp.cars) {
        out << car << ' ';
    }
    out << "\n";
    return out;
}
istream& operator>>(istream& in, employee& emp) {
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
int employee::employeeNextId=0;
class customer: public User{
    public:
    static int customerNextId;
    static int avgcustomerRating;
    customer(const string& namenew, const string& passwordnew){
        name=namenew;
        password=passwordnew;
        position="Customer";
        id="C"+to_string(customerNextId++);
        if (namenew==""||passwordnew==""){
            customerNextId--;
        }
        cars.push_back(-1);
        numberofCars=0;
        record=avgcustomerRating;
        Payable=0;
    }
    bool canRent() {
        if (record<=-50) {
            cout<<"Bad record.\n";
            return false;
        }
        else {
            int limit;
            if (record>=40){
                limit=7;
            }
            else if (record>=-20) limit=(record+40)/10;
            else limit=1;
            if (this->numberofCars>=limit) cout <<"Maximum cars rented.\n";
                return false;
        }
        return true;
    }
    void rentCar(int carId, int startdate,int duedate) {
        Car car = getCar(carId);
        if (car.ownerId=="") {
            cout << "Car not found.\n";
            return;
        }
        
        if (!car.isAvailable(startdate)) {
            cout << "Car is not available.\n";
            return;
        }
        if (!canRent()) {
            cout << "Customer cannot rent more cars.\n";
            return;
        }
        car.RentRequest(id,duedate);
        updateCar(car);
        this->addCar(carId);
        this->Payable += (car.dailyRate)*(duedate-startdate);
        cout << "Car rented successfully.\n";
    }
    void returnCar(int carId, int current_date) {
        Car car = getCar(carId);
        if (car.ownerId=="") {
            cout << "Car not found.\n";
            return;
        }
        if (car.ownerId != this->id) {
            cout << "Car is not rented by you.\n";
            return;
        }
        if (car.dueDate < current_date) {
            this->Payable += (car.dailyRate)*(current_date-car.dueDate)*1.25;
            this->record -= 10 * (current_date - car.dueDate);
        }
        else if (car.dueDate > current_date){
            this->record += 10*(car.dueDate - current_date);
        }
        car.ownerId = "M";
        car.dueDate = 0;
        updateCar(car);
        this-> removeCar(carId);
        cout << "Car returned successfully.\n";
    }
    friend ostream& operator<<(ostream& out, const customer& cst);
    friend istream& operator>>(istream& in, customer& cst);
};
int customer::customerNextId=0;
int customer::avgcustomerRating=0;
ostream& operator<<(ostream& out, const customer& cst) {
    out << cst.id << ' ' << cst.name << ' ' << cst.password<<' '<<cst.position << ' ' << cst.record << ' ' << cst.Payable << ' '<<cst.numberofCars<<' ';
    for (const auto& car : cst.cars) {
        out << car << ' ';
    }
    out << "\n";
    return out;
}
istream& operator>>(istream& in, customer& cst) {
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
vector<employee> employees;
vector<customer> customers;
employee getEmployee(string id){
        for (auto i:employees){
            if (i.getId()==id){
                return i;
            }
        }
        employee emp("","");
        return emp;
    }
customer getCustomer(string id){
        for (auto& i:customers){
            if (i.getId()==id){
                return i;
            }
        }
        customer cust("","");
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
                cout << "Car deleted successfully.\n";
                return;
            }
        }
        cout << "Car not found.\n";
    }
    void displayAllCars(){
        if (cars.size()==0){
            cout<<"No cars found.\n";
        }
        for (auto i:cars){
            i.display();
        }
    }
    void displayAllEmployees(){
        if (employees.size()==0){
            cout<<"No employees found.\n";
        }
        for (auto i:employees){
            i.display();
        }
    }
    void displayAllCustomers(){
        if (customers.size()==0){
            cout<<"No customers found.\n";
        }
        for (auto i:customers){
            i.display();
        }
    }
    void deleteCustomer(string id){
        for (auto it = customers.begin(); it != customers.end(); ++it) {
            if (it->getId() == id) {
                customer cust = *it;
                for (auto& car : cust.cars) {
                    Car carobj = getCar(car);
                    carobj.RentRequest("M",0); // Make it available for rent again
                    updateCar(carobj);
                }
                customers.erase(it);
                return;
            }
        }
        cout << "Customer not found.\n";
    }
    void addEmployee(const employee& emp) {
        employees.push_back(emp);
    }
    void addCustomer(const customer& cust) {
        customers.push_back(cust);
    }
    void deleteEmployee(string id){
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getId() == id) {
                employees.erase(it);
                return;
            }
        }
        cout << "Employee not found.\n";
    }
    friend class customer;
    friend class employee;
};
void saveCarsToFile(const string& fileName) {
        ofstream outFile(fileName, ios::trunc);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& car : cars) {
            outFile << car;
        }
        outFile.close();
    }
void loadCarsFromFile(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            cerr << "Error opening file for reading.\n";
            return;
        }
        Car car;
        while (inFile >> car) {
            cars.push_back(car);
            Car::nextId = max(Car::nextId, car.getId() + 1);
        }
        inFile.close();
    }
void saveEmployeesToFile(const string& fileName) {
        ofstream outFile(fileName, ios::trunc);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& u : employees) {
                outFile << u;
        }
        outFile.close();
    }
void saveCustomersToFile(const string& fileName) {
        ofstream outFile(fileName, ios::trunc);
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }
        for (const auto& u : customers) {
                outFile << u;
        }
        outFile.close();
    }
void loadEmployeesFromFile(const string& fileName) {
        ifstream inFile(fileName);

        if (!inFile) {
            cerr << "Error opening file for reading.\n";
            return;
        }
        employee emp = employee("",""); // create a dummy employee to initialize the next id
        while (inFile >> emp) {
            employees.push_back(emp);
            employee::employeeNextId = max(employee::employeeNextId, stoi(emp.getId().substr(1)) + 1);
        }
        inFile.close();
    }
void loadCustomersFromFile(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            cerr << "Error opening file for reading.\n";
            return;
        }
        customer cust("",""); // create a dummy customer to initialize the next id
        while (inFile >> cust) {
            customers.push_back(cust);
            customer::customerNextId = max(customer::customerNextId, stoi(cust.getId().substr(1)) + 1);
            customer::avgcustomerRating+=cust.record;
        }
        customer::avgcustomerRating/=customers.size();

        inFile.close();
    }
void displayManagerchoices(){
    cout<<"Enter your choice:\n";
    cout<<"0. Change Password";
    cout<<"1. Add a car\n";
    cout<<"2. Delete a car\n";
    cout<<"3. Display all cars\n";
    cout<<"4. Add an employee\n";
    cout<<"5. Delete an employee\n";
    cout<<"6. Display all employees\n";
    cout<<"7. Add a customer\n";
    cout<<"8. Delete a customer\n";
    cout<<"9. Display all customers\n";
    cout<<"10. Update an Employee\n";
    cout<<"11. Update a Customer\n";
    cout<<"12. Update a Car\n";
    cout<<"13. Exit\n";
}
void displayEmployeeChoices(){
    cout<<"Enter your choice:\n";
    cout<<"1. Rent a car\n";
    cout<<"2. Return a car\n";
    cout<<"3. Display all Available cars\n";
    cout<<"4. Display all your cars\n";
    cout<<"5. Clear due\n";
    cout<<"7. Exit\n";
}
void displayCustomerChoices(){
    cout<<"Enter your choice:\n";
    cout<<"1. Rent a car\n";
    cout<<"2. Return a car\n";
    cout<<"3. Display all cars\n";
    cout<<"4. Display all your cars\n";
    cout<<"5. Clear due\n";
    cout<<"7. Exit\n";
}
void updateCar(Car car){
    for (auto& i:cars){
        if (i.getId()==car.getId()){
            i=car;
            return;
        }
    }
}
void updateEmployee(employee emp){
    for (auto& i:employees){
        if (i.getId()==emp.getId()){
            i=emp;
            return;
        }
    }
}
void updateCustomer(customer cust){
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
    for (auto i:cars){
        if (i.isAvailable(date)){
            i.display();
            counter++;
        }
    }
    if (counter==0){
        cout<<"No cars available at the given date. Sorry for your inconvenience!\n";
    }
    cout<<endl;
}
int main() {
    Manager database;
    string Manager_pass="Mrental001";
    loadCarsFromFile("cars.txt");
    loadEmployeesFromFile("employees.txt");
    loadCustomersFromFile("customers.txt");
    cout<<"Welcome to the car rental system.\n";
    cout<<"Please enter your position (Manager, Employee, Customer): ";
    string position;
    cin>>position;
    if (position=="Manager"){
        cout<<"Please enter the password: ";
        string pass;
        cin>>pass;
        int counter=0;
        while (pass!=Manager_pass){
            cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                cout<<"Too many incorrect attempts.\n";
                return 0;
            }
        }
        cout<<"Welcome, Manager.\n";
        int choice;
        displayManagerchoices();
        cin>>choice;
        while (choice!=13){
            if (choice==0){
                cout<<"Enter the new password: ";
                cin>>Manager_pass;
                while (Manager_pass.length()<8){
                    cout<<"Password must be at least 8 characters long.\n";
                    cout<<"Enter the password of the customer: ";
                    cin>>Manager_pass;
                }
                cout<<"Password changed successfully.\n";
            }
            if (choice==1){
                string make,model,color,Condition;
                int year;
                double dailyRate;
                cout<<"Enter the make of the car: ";
                cin>>make;
                cout<<"Enter the model of the car: ";
                cin>>model;
                cout<<"Enter the color of the car: ";
                cin>>color;
                cout<<"Enter the Condition of the car: (G: Good, B: Bad, D: Doable)";
                cin>>Condition;
                cout<<"Enter the year of the car: ";
                cin>>year;
                cout<<"Enter the daily rate of the car: ";
                cin>>dailyRate;
                Car car(make,model,color,Condition,year,dailyRate);
                database.addCar(car);
                cout << "Car added successfully.\n";
                
            }
            else if (choice==2){
                int id;
                cout<<"Enter the id of the car: ";
                cin>>id;
                database.deleteCar(id);
                cout << "Car deleted successfully.\n";
            }
            else if (choice==3){
                database.displayAllCars();
            }
            else if (choice==4){
                string name,pass;
                cout<<"Enter the name of the employee: ";
                cin>>name;
                cout<<"Enter the password of the employee: ";
                cin>>pass;
                while (pass.length()<8){
                    cout<<"Password must be at least 8 characters long.\n";
                    cout<<"Enter the password of the customer: ";
                    cin>>pass;
                }
                employee emp(name,pass);
                database.addEmployee(emp);
                cout << "Employee added successfully.\n";
            }
            else if (choice==5){
                string id;
                cout<<"Enter the id of the employee: ";
                cin>>id;
                database.deleteEmployee(id);
                cout << "Employee deleted successfully.\n";
            }
            else if (choice==6){
                database.displayAllEmployees();
            }
            else if (choice==7){
                string name,pass;
                cout<<"Enter the name of the customer: ";
                cin>>name;
                cout<<"Enter the password of the customer: ";
                cin>>pass;
                while (pass.length()<8){
                    cout<<"Password must be at least 8 characters long.\n";
                    cout<<"Enter the password of the customer: ";
                    cin>>pass;
                }
                customer cust(name,pass);
                database.addCustomer(cust);
                cout << "Customer added successfully.\n";
            }
            else if (choice==8){
                string id;
                cout<<"Enter the id of the customer: ";
                cin>>id;
                database.deleteCustomer(id);
                cout << "Customer deleted successfully.\n";
            }
            else if (choice==9){
                database.displayAllCustomers();     
            }
            else if (choice==10){
                string id;
                cout<<"Enter the id of the employee: ";
                cin>>id;
                employee emp=getEmployee(id);
                if (emp.getname()=="" && emp.getpass()==""){
                    cout<<"Employee not found.\n";
                }
                else {
                emp.display();
                cout<<"Do you want to update the record? (Y/N): ";
                char ch;
                cin>>ch;
                if (ch=='Y'){
                    cout<<"Enter the new record: ";
                    int record;
                    cin>>record;
                    emp.record=record;
                    emp.display();
                }
                updateEmployee(emp);
                cout<<"Do you want to update the payable? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    cout<<"Enter the new payable: ";
                    int payable;
                    cin>>payable;
                    emp.Payable=payable;
                    emp.display();
                }
                updateEmployee(emp);
                cout<<"Do you want to remove a car? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    int carId;
                    cout<<"Enter the id of the car: ";
                    cin>>carId;
                    emp.removeCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        cout<<"Car not found.\n";
                    }
                    else car.RentRequest("M",0); // Make it available for rent again
                    emp.display();
                }
                updateEmployee(emp);
                cout<<"Do you want to add a car? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    int carId;
                    cout<<"Enter the id of the car: ";
                    cin>>carId;
                    emp.addCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        cout<<"Car not found.\n";
                    }
                    else if (car.ownerId!="M"){
                        cout<<"Car already rented.\n";
                    }
                    else {
                        cout<<"Enter the due date: ";
                        int duedate;
                        cin>>duedate;
                        while (isValidDate(duedate)==false){
                            cout<<"Invalid date.\n";
                            cin>>duedate;
                        }
                        car.RentRequest(emp.getId(),duedate);
                    } // Make it unavailable for rent
                    emp.display();
                }
                updateEmployee(emp);
                }
            }
            else if (choice==11){
                string id;
                cout<<"Enter the id of the customer: ";
                cin>>id;
                customer cust=getCustomer(id);
                if (cust.getname()=="" && cust.getpass()==""){
                    cout<<"Customer not found.\n";
                }
                else {
                cust.display();
                cout<<"Do you want to update the record? (Y/N): ";
                char ch;
                cin>>ch;
                if (ch=='Y'){
                    cout<<"Enter the new record: ";
                    int record;
                    cin>>record;
                    cust.record=record;
                    cust.display();
                }
                updateCustomer(cust);
                cout<<"Do you want to update the payable? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    cout<<"Enter the new payable: ";
                    int payable;
                    cin>>payable;
                    cust.Payable=payable;
                    cust.display();
                }
                updateCustomer(cust);
                cout<<"Do you want to remove a car? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    int carId;
                    cout<<"Enter the id of the car: ";
                    cin>>carId;
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        cout<<"Car not found.\n";
                    }
                    else if (car.ownerId!=cust.getId()){
                        cout<<"Car not rented by the customer.\n";
                    }
                    else {
                    cust.removeCar(carId);
                    car.RentRequest("M",0);
                    } // Make it available for rent again
                    cust.display();
                }
                updateCustomer(cust);
                cout<<"Do you want to add a car? (Y/N): ";
                cin>>ch;
                if (ch=='Y'){
                    int carId;
                    cout<<"Enter the id of the car: ";
                    cin>>carId;
                    cust.addCar(carId);
                    Car car=getCar(carId);
                    if (car.getId()==-1){
                        cout<<"Car not found.\n";
                    }
                    else {
                        if (car.ownerId!="M"){
                            cout<<"Car already rented.\n";
                        }
                        else {
                        cout<<"Enter the due date: ";
                            int duedate;
                            cin>>duedate;
                            while (isValidDate(duedate)==false){
                                cout<<"Invalid date.\n";
                                cin>>duedate;
                            }
                            car.RentRequest(cust.getId(),duedate);
                        }
                    } // Make it unavailable for rent
                    cust.display();
                }
                updateCustomer(cust);
                }
            }
            else if (choice==12){
                int id;
                cout<<"Enter the id of the car: ";
                cin>>id;
                Car car=getCar(id);
                if (car.getId()==-1){
                    cout<<"Car not found.\n";
                    return 0;
                }
                car.display();
                cout<<"Do you want to update the due date? (Y/N): ";
                char ch;
                cin>>ch;
                if (ch=='Y'){
                    cout<<"Enter the new due date: ";
                    int duedate;
                    cin>>duedate;
                    car.setDueDate(duedate);
                    car.display();
                }
                updateCar(car);
                cout << "Car updated successfully.\n";
                cout << "Do you want to update the price? (Y/N): ";
                cin >> ch;
                if (ch == 'Y') {
                    cout << "Enter the new daily rate: ";
                    double dailyRate;
                    cin >> dailyRate;
                    car.dailyRate = dailyRate;
                    car.display();
                }
                updateCar(car);

            }
            else cout<<"Invalid choice.\n";
            displayManagerchoices();
            cin>>choice;
        }
    }
    else if (position=="Employee"){
        cout<<"Enter your id: ";
        string id;
        cin>>id;
        employee emp=getEmployee(id);
        if (emp.getname()=="" && emp.getpass()==""){
            cout<<"Employee not found.\n";
            return 0;
        }
        cout<<"Enter your password: ";
        string pass;
        cin>>pass;
        int counter=0;
        while (emp.checkpass(pass)==false){
            cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                cout<<"Too many incorrect attempts.\n";
                return 0;
            }
            cout<<"Please enter your password again:\n";
            cin>>pass;
        }
        cout<<"Welcome," << emp.getname() <<".\n";
        emp.display();
        int choice;
        displayEmployeeChoices();
        cin>>choice;
        while (choice!=7){
            if (choice==1){
                int carId,startdate,duedate;
                cout<<"Enter the id of the car: ";
                cin>>carId;
                cout<<"Enter the start date (in YYYYMMDD format):";
                cin>>startdate;
                while (isValidDate(startdate)==false){
                    cout<<"Invalid date.\n";
                    cin>>startdate;
                }
                cout<<"Enter the due date (YYYYMMDD format):";
                cin>>duedate;
                while (duedate<startdate||isValidDate(duedate)==false){
                    cout<<"Invalid date. Enter correct date\n";
                    cin>>duedate;
                }
                emp.rentCar(carId,startdate,duedate);
            }
            else if (choice==2){
                int carId,current_date;
                cout<<"Enter the id of the car: ";
                cin>>carId;
                cout<<"Enter the current date(YYYYMMDD format): ";
                cin>>current_date;
                while (isValidDate(current_date)==false){
                    cout<<"Invalid date.\n";
                    cin>>current_date;
                }
                emp.returnCar(carId,current_date);
            }
            else if (choice==3){
                int date;
                cout<<"Enter the date (YYYYMMDD format): ";
                cin>>date;
                while (isValidDate(date)==false){
                    cout<<"Invalid date.\n";
                    cin>>date;
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
                cout<<"Invalid choice.\n";
            }
            displayEmployeeChoices();
            emp.display();
            cin>>choice;
            updateEmployee(emp);
        }
        
    }
    else if (position=="Customer"){
        cout<<"Enter your id: ";
        string id;
        cin>>id;
        customer cust=getCustomer(id);
        if (cust.getname()=="" && cust.getpass()==""){
            cout<<"Customer not found.\n";
            cout << "Do you want to register as a new customer? (Y/N): ";
            char ch;
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                cout << "Enter your name: ";
                string name;
                cin >> name;
                cout << "Enter your password: ";
                string pass;
                cin >> pass;
                while (pass.length() < 8) {
                    cout << "Password must be at least 8 characters long.\n";
                    cout << "Enter the password of the customer: ";
                    cin >> pass;
                }
                customer custnew(name, pass);
                database.addCustomer(custnew);
                cout << "Customer added successfully.\n";
                saveCustomersToFile("customers.txt");
                cust = custnew;
            }
            else return 0;
        }
        cout<<"Enter your password: ";
        string pass;
        cin>>pass;
        int counter=0;
        while (cust.checkpass(pass)==false){
            cout<<"Incorrect password.\n";
            counter++;
            if (counter>=3){
                cout<<"Too many incorrect attempts.\n";
                return 0;
            }
            cout<<"Please enter your password again:\n";
            cin>>pass;
        }
        cout<<"Welcome, "<<cust.getname()<<".\n";
        cust.display();
        int choice;
        displayCustomerChoices();
        cin>>choice;
        while (choice!=7){
            if (choice==1){
                int carId,startdate,duedate;
                cout<<"Enter the id of the car: ";
                cin>>carId;
                cout<<"Enter the start date (in YYYYMMDD format):";
                cin>>startdate;
                while (isValidDate(startdate)==false){
                    cout<<"Invalid date.\n";
                    cin>>startdate;
                }
                cout<<"Enter the due date (YYYYMMDD format):";
                cin>>duedate;
                while (duedate<startdate||isValidDate(duedate)==false){
                    cout<<"Invalid date. Enter correct date\n";
                    cin>>duedate;
                }
                cust.rentCar(carId,startdate,duedate);
            }
            else if (choice==2){
                int carId,current_date;
                cout<<"Enter the id of the car: ";
                cin>>carId;
                cout<<"Enter the current date: ";
                cin>>current_date;
                while (isValidDate(current_date)==false){
                    cout<<"Invalid date.\n";
                    cin>>current_date;
                }
                cust.returnCar(carId,current_date);
            }
            else if (choice==3){
                int date;
                cout<<"Enter the date (YYYYMMDD format): ";
                cin>>date;
                while (isValidDate(date)==false){
                    cout<<"Invalid date.\n";
                    cin>>date;
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
                cout<<"Invalid choice.\n";
            }
            cust.display();
            displayCustomerChoices();
            cin>>choice;
            updateCustomer(cust);
        }
    }
    else cout<<"Invalid position.\n";
    saveCarsToFile("cars.txt");
    saveEmployeesToFile("employees.txt");
    saveCustomersToFile("customers.txt");
    cout<<"Thank you for using the car rental system.\n";
    return 0;
}
