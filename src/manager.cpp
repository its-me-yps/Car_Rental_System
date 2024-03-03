#include <iostream>
#include <string>
#include <fstream>
#include <pqxx/pqxx>
#include "../include/user.h"
#include "../include/car.h"

void managerOperations(Manager& manager) {
    std::cout << "Manager logged in. Choose an operation:" << std::endl;
    std::cout << "[1] Add Customer" << std::endl;
    std::cout << "[2] Update Customer" << std::endl;
    std::cout << "[3] Delete Customer" << std::endl;
    std::cout << "[4] Add Employee" << std::endl;
    std::cout << "[5] Update Employee" << std::endl;
    std::cout << "[6] Delete Employee" << std::endl;
    std::cout << "[7] Add Car" << std::endl;
    std::cout << "[8] Update Car" << std::endl;
    std::cout << "[9] Delete Car" << std::endl;
    std::cout << "[10] View All Cars" << std::endl;
    std::cout << "[11] Car Returned by a User" << std::endl;
    std::cout << "[12] Exit" << std::endl;

    int operation;
    std::cout << "Enter operation number: ";
    std::cin >> operation;

    switch (operation) {
        case 1: {
            std::string name, ID, password;
            std::cout << "Enter customer Name: ";
            std::cin >> name;
            std::cout << "Enter customer ID: ";
            std::cin >> ID;
            std::cout << "Enter customer password: ";
            std::cin >> password;
            manager.AddCustomer(name, ID, password);
            break;
        }
        case 2: {
            std::string Name, ID, password;
            std::cout << "Enter customer ID: ";
            std::cin >> ID;
            std::cout << "Enter updated customer name: ";
            std::cin >> Name;
            std::cout << "Enter updated customer password: ";
            std::cin >> password;
            manager.UpdateCustomer(ID, Name, password);
            break;
        }
        case 3: {
            std::string ID;
            std::cout << "Enter customer ID to delete: ";
            std::cin >> ID;
            manager.DeleteCustomer(ID);
            break;
        }
        case 4: {
            std::string name, ID, password;
            std::cout << "Enter employee Name: ";
            std::cin >> name;
            std::cout << "Enter employee ID: ";
            std::cin >> ID;
            std::cout << "Enter employee password: ";
            std::cin >> password;
            manager.AddEmployee(name, ID, password);
            break;
        }
        case 5: {
            std::string Name, ID, password;
            std::cout << "Enter employee ID: ";
            std::cin >> ID;
            std::cout << "Enter updated employee name: ";
            std::cin >> Name;
            std::cout << "Enter updated employee password: ";
            std::cin >> password;
            manager.UpdateCustomer(ID, Name, password);
            break;
        }
        case 6: {
            std::string employeeID;
            std::cout << "Enter employee ID to delete: ";
            std::cin >> employeeID;
            manager.DeleteEmployee(employeeID);
            break;
        }
        case 7: {
            std::string model, other_details;
            int condition, dailyCharge, dailyFine;
            std::cout << "Enter car model: ";
            std::cin >> model;
            std::cout << "Enter car condition on a scale of 1(broken) - 10(working): ";
            std::cin >> condition;
            if (condition < 1 || condition > 10) {
                std::cerr << "Error: Invalid Input" << std::endl;
                exit(0);
            }
            std::cout << "Enter other details (if any): ";
            std::cin >> other_details;
            std::cout << "Enter daily charge: ";
            std::cin >> dailyCharge;
            std::cout << "Enter daily fine: ";
            std::cin >> dailyFine;
            
            manager.AddCar(model, condition, other_details, dailyCharge, dailyFine);
            break;
        }
        case 8: {
            std::string model, other_details;
            int condition, dailyCharge, dailyFine;
            std::cout << "Enter car model: ";
            std::cin >> model;
            std::cout << "Enter updated car condition: ";
            std::cin >> condition;
            if (condition < 1 || condition > 10) {
                std::cerr << "Error: Invalid Input" << std::endl;
                exit(0);
            }
            std::cout << "Enter updated car details: ";
            std::cin >> other_details;
            std::cout << "Enter updated daily charge: ";
            std::cin >> dailyCharge;
            std::cout << "Enter updated daily fine: ";
            std::cin >> dailyFine;
            
            manager.UpdateCar(model, condition, other_details, dailyCharge, dailyFine);
            break;
        }
        case 9: {
            std::string model;
            std::cout << "Enter car model to delete: ";
            std::cin >> model;
            manager.DeleteCar(model);
            break;
        }
        case 10: {
            viewAllCars(manager.conn);
        }
        case 11: {
            std::string ID,model;
            int option;
            std::cout << "Who is Returning Car ? [1]Customer [2]Employee ";
            std::cin >> option;
            std::cout << "Enter ID ";
            std::cin >> ID;
            std::cout << "Car Model ";
            std::cin >> model;
            if(option != 1 || option != 2) {
                break;
            }
            manager.clearDues(ID, model, option);
        }
        case 12: {
            std::cout << "Exited" << std::endl;
            exit(0);
        }
        default:
            std::cerr << "Invalid operation number." << std::endl;
            // exit(0);
    }
    managerOperations(manager);
}