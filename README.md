# Car Rental Software

## Overview 
This is a car rental application . The entire application has been coded in *C++*. For the database management *C++ file handling* has been used. The interaction is to be done through the Command line Interface. The system has been tested to be working properly on  *UNIX* based systems. 

## Structure of Databases

- There are 3 databases that have been used as given in the Problem Statement with the liberty of making a single Manager.
- The customers.txt and employees.txt have the following entries 
⁠UserId,Name,Password,Payable,Record, Number of Rented Cars, List of IDs of rented Cars. ⁠
- The cars.txt has the following entries.
⁠ID,Make,Model,Color,Condition,OwnerId ("M" in case of Not rented),DueDate (0 in case of not rented),Cost per day,year.
-The Manager has the password "Mrental001" for signing in.

## Running the Code 
- Compile the code using ```G++``` compiler with version >=11.0 and Run the rental.exe file.
- On entry, you will be asked if you are the Manager, an Employee, or a Customer. Note that you can only register if you are a customer. Upon registering the record of a customer is the average customer record prior to the running of the application. If none existing customers, it will be 0.
- Further you will have to login using your password which has a maximum of 3 failed attempts after which the  code exits.
- After logging in you will be provided with menus to perform the required tasks. namely updating the database for the Manager and renting/returning a car for Customer or Employee. You can also clear your dues.

### Renting a Car
For renting a car you must have a good record threshold first. Further your record will decide the number io cars you can rent. A good record can help you rent more cars but not more than a threshold. The criteria is different for employees and customers thus eliminating the need to have separate attributes for them. This can only be cleared by the manager. You also will not be able to rent if you have Payable more than a threshold. Returning a car does not mean you have made the payment. You must do it separately. Once rented, the payable will update in your account and show immediately. Employees get a discount of 15%.

### Late Payment 
If late, a customer has to give 25% of the daily rate as penalty for each day. His/her record will also get affected by the same. This is applied on the return of a car. 

## Assumptions
- I assume that the customers/employees will enter Name and password without spaces for smooth Handling. 
- I assume that the user will give legit date input. 
- I assume that upon clearing dues, the customer actually pays.
- I assume that the user gives an integer input when asked.# Car_Rental_System
