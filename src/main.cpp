#include <iostream>
#include "database.cpp"
#include "authenticate.cpp"
#include "manager.cpp"
#include "read.cpp"
#include "user.cpp"
#include "../include/user.h"
#include "../include/car.h"

int main(int argc, char* argv[]) {

    Credentials credentials;
    try{
        credentials = readSecretFile("./secret.txt", "db_user", "db_password");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    pqxx::connection conn = init_Db(credentials.username, credentials.password, "localhost", "car_rentals");
    createTables(conn);

    while(1) {
        std::cout << std::endl;
        std::cout << "Welcome to Car Rentals. Login as [1] Customer [2] Employee [3] Manager [4] Exit" << std::endl;
        int option;
        std::cout << "Choose an option: ";
        std::cin >> option;

        if(option == 4) {
            std::cout << "Exited" << std::endl;
            return 0;
        }

        if(option > 4) {
            std::cout << "Invalid Option Chosen" << std::endl;
            continue;;
        }

        std::string id, password;
        std::cout << "Enter ID: ";
        std::cin >> id;
        std::cout << "Enter password: ";
        std::cin >> password;

        // Authenticate user
        try {
            if(option != 3) {
                authenticateUser(conn, option, id, password);
                User* u = getUser(option, id, conn);
                userOperations(*u, conn);
            }
            else {
                Credentials manager_credentials = readSecretFile("./secret.txt", "manager_ID", "manager_Password");
                if(id != manager_credentials.username || password != manager_credentials.password) {throw std::runtime_error("Wrong Credentials, Login Failed");}

                Manager manager(id, password, "Manager", conn);
                managerOperations(manager);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
        }
    }
}