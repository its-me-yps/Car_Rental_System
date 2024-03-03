#include <pqxx/pqxx>
#include <iostream>
#include "../include/user.h"
#include "../include/car.h"

void userOperations(User &u, pqxx::connection& conn){
    std::cout << "Logged in. Choose an operation:" << std::endl;
    std::cout << "[1] See My Records" << std::endl;
    std::cout << "[2] Rent Car" << std::endl;
    std::cout << "[3] View My Rented Cars" << std::endl;
    std::cout << "[4] View All Cars" << std::endl;
    std::cout << "[5] Exit" << std::endl;

    int operation;
    std::cout << "Enter operation number: ";
    std::cin >> operation;

    // Calculating Fine before operations
    u.calculateFine(conn);

    switch(operation) {
        case 1: {
            u.viewFine();
            u.seeRecords();
            break;
        }
        case 2: {
            std::string model;
            std::cout << "Name of the Model you want to Rent: " << std::endl;
            std::cin >> model;
            u.rentRequest(model, conn);
            break;
        }
        case 3: {
            u.rentedCars();
            break;
        }
        case 4: {
            viewAllCars(conn);
        }
        case 5: {
            exit(0);
        }
        default: {
            std::cout << "Invalid operation." << std::endl;
            break;
        }
    }
    userOperations(u, conn);
}

User* getUser(int option, const std::string& id, pqxx::connection& conn) {
    if (option == 1) {
        std::string query = "SELECT * FROM customers WHERE id = " + conn.quote(id);
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(query);
        if (!result.empty()) {
            std::string name = result[0]["name"].as<std::string>();
            std::string password = result[0]["password"].as<std::string>();

            // Construct the Rented_cars vector
            std::vector<std::string> rentedCars;
            std::string rentedCarsStr = result[0]["rented_cars"].as<std::string>();
            std::stringstream ss(rentedCarsStr);
            std::string car;
            while (std::getline(ss, car, ',')) {
                rentedCars.push_back(car);
            }

            return new Customer(name, id, password, rentedCars);
        } else {
            throw std::runtime_error("Customer not found in the database.");
        }
    } else if (option == 2) {
        std::string query = "SELECT * FROM employees WHERE id = " + conn.quote(id);
        pqxx::work txn(conn);
        pqxx::result result = txn.exec(query);
        if (!result.empty()) {
            std::string name = result[0]["name"].as<std::string>();
            std::string password = result[0]["password"].as<std::string>();

            // Construct the Rented_cars vector
            std::vector<std::string> rentedCars;
            std::string rentedCarsStr = result[0]["rented_cars"].as<std::string>();
            std::stringstream ss(rentedCarsStr);
            std::string car;
            while (std::getline(ss, car, ',')) {
                rentedCars.push_back(car);
            }

            return new Employee(name, id, password, rentedCars);
        } else {
            throw std::runtime_error("Employee not found in the database.");
        }
    } else {
        throw std::invalid_argument("Invalid option specified.");
    }
}
