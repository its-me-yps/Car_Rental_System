#ifndef CAR_H
#define CAR_H

#include <string>
#include <pqxx/pqxx>
#include <chrono>
#include <ctime>
#include <iomanip>

class Car {
    std::string Model;
    int Condition;
    std::string other_details;
    int due_Date;
    bool rented_Status;
    int daily_Charges;
    int daily_Fine;

    public:
        void Show_duedate(){};
        bool rentedOrNot() {
            return rented_Status;
        }
};

void viewAllCars(pqxx::connection& conn) {
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec("SELECT * FROM cars");

        for (const auto& row : result) {
            std::string model = row["Model"].as<std::string>();
            int condition = row["Condition"].as<int>();

            std::string rentedStatus = "Available";
            pqxx::result rentResult = txn.exec("SELECT * FROM customers WHERE '" + model + "' = ANY(Rented_cars)");
            if (!rentResult.empty()) {
                rentedStatus = "Rented";
            }

            std::string dueDate = (rentedStatus == "Rented") ? "Due Date" : "";

            std::cout << "Model: " << model << ", Condition: " << condition << ", Status: " << rentedStatus << ", " << dueDate << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

#endif // CAR_H