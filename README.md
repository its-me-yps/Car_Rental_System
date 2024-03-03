# Car Rental System

## Project Structure

### include
This directory contains header files defining classes and their methods. Specifically, there are two files: "car.h" and "user.h". The "Car" class represents a car, while the "User" class is an abstract class with three derived classes: "Customer", "Employee", and "Manager". Additionally, the "rent.h" file is included to handle rent-related operations and database insertions.

### src
The "src" directory contains the implementation of the user interface, effectively managing user interactions and calling appropriate methods. This directory contains multiple source files.

### secret.txt
This file stores sensitive information such as the PostgreSQL username and password, as well as the manager's credentials. It serves as a means to securely access the database. The "read.cpp" file is responsible for reading data from "secret.txt".

## Database
The project utilizes PostgreSQL as its database system. The "libpqxx" library is employed as the C++ driver for PostgreSQL. The application interacts with the database using the "pqxx::connection" object obtained from the driver.

## Functionalities
Upon starting the application for the first time, it automatically creates empty database tables. The user is then prompted to log in as either a "Customer", "Employee", "Manager", or to exit the program.

If logging in as a manager, the user must provide their manager ID and password (should be same as one in secret.txt or program will terminate with Wrong Credentials). Once authenticated, the manager gains access to a range of options, such as adding customers, employees, and cars to the system.

Exiting the manager session allows users to log in as a "Customer" or an "Employee". Customers and employees can perform various actions, including renting cars, viewing their records, and checking the availability of cars.

Additionally, all users, regardless of their role, can view all cars in the system, distinguishing between available and rented vehicles.