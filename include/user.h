#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "rent.h"

class User {
protected:
    std::string Name;
    std::string ID;
    std::string Password;

public:
    User(const std::string& name, const std::string& id, const std::string& password)
        : Name(name), ID(id), Password(password) {}

    virtual void viewFine() = 0;
    virtual void rentRequest(const std::string& model, pqxx::connection& conn) = 0;
    virtual void rentedCars() = 0;
    virtual void seeRecords() = 0;
    virtual void calculateFine(pqxx::connection& conn) = 0;
};

class Customer : public User {
    std::vector<std::string> Rented_cars;
    int Fine_due;
    std::string record;
    std::string Other_details;

public:
    Customer(const std::string& name, const std::string& id, const std::string& password, const std::vector<std::string>& rentedCars)
    : User(name, id, password), Rented_cars(rentedCars) {}

    void viewFine() override {
        std::cout << "Fine Due: " << Fine_due << std::endl;
    }

    void rentRequest(const std::string& model, pqxx::connection& conn) override {
        if(record == "BAD") {
            std::cerr << "You have a Bad Customer Record and are not allowed to Rent another Car!!\nReturn rented cars and pay the fine to avoid lawsuit." << std::endl;
        }
        return;
        if(Request(model, conn, 0)) {
            Rented_cars.push_back(model);
            try {
                pqxx::work txn(conn);
                std::string updateQuery = "UPDATE customers SET Rented_cars = ";
                updateQuery += "{";
                for (size_t i = 0; i < Rented_cars.size(); ++i) {
                    updateQuery += "'" + Rented_cars[i] + "'";
                    if (i != Rented_cars.size() - 1) {
                        updateQuery += ",";
                    }
                }
                updateQuery += "} WHERE ID = '" + ID + "'";

                txn.exec(updateQuery);
                txn.commit();
            } catch (const std::exception& e) {
                std::cerr << "Error updating rented cars: " << e.what() << std::endl;
            }
        }
    }

    void rentedCars() override {
        std::cout << "Rented Cars: ";
        for (const auto& car : Rented_cars) {
            std::cout << car << " ";
        }
        std::cout << std::endl;
    }

    void seeRecords() override {
        std::cout << "Records: " << record << std::endl;
    }

    void calculateFine(pqxx::connection& conn) override {
        try {
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
            int totalFine = 0;
            int overdueCarsCount = 0;
            int totalCarsCount = Rented_cars.size();

            for (const auto& carModel : Rented_cars) {
                pqxx::work txn(conn);
                std::string query = "SELECT * FROM cars WHERE model = " + conn.quote(carModel);
                pqxx::result result = txn.exec(query);
                txn.commit();

                if (!result.empty()) {
                    std::string dueDateStr = result[0]["due_date"].as<std::string>();
                    std::tm dueDate = {};
                    std::istringstream ss(dueDateStr);
                    ss >> std::get_time(&dueDate, "%Y-%m-%d %H:%M:%S");

                    auto dueDateTime = std::chrono::system_clock::from_time_t(std::mktime(&dueDate));

                    if (now > dueDateTime) {
                        auto overdueDuration = std::chrono::duration_cast<std::chrono::minutes>(now - dueDateTime);
                        int dailyFine = result[0]["daily_fine"].as<int>();
                        int fine = dailyFine * static_cast<int>(overdueDuration.count());

                        totalFine += fine;
                        overdueCarsCount++;
                    }
                }
            }

            pqxx::work updateTxn(conn);
            Fine_due = totalFine;
            updateTxn.exec("UPDATE customers SET Fine_due = " + std::to_string(totalFine) + " WHERE ID = '" + ID + "'");

            std::string customerRecord = (overdueCarsCount >= totalCarsCount / 2) ? "BAD" : "GOOD";
            record = customerRecord;
            updateTxn.exec("UPDATE customers SET customer_record = '" + customerRecord + "' WHERE ID = '" + ID + "'");
            
            updateTxn.commit();
            
            std::cout << "Fine calculated successfully. Total fine due: " << totalFine << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
};

class Employee : public User {
    std::vector<std::string> Rented_cars;
    int Fine_due;
    std::string record;
    std::string Other_details;

public:
    Employee(const std::string& name, const std::string& id, const std::string& password, const std::vector<std::string>& rentedCars)
    : User(name, id, password), Rented_cars(rentedCars) {}

    void viewFine() override {
        std::cout << "Fine Due: " << Fine_due << std::endl;
    }

    void rentRequest(const std::string& model, pqxx::connection& conn) override {
        if(Request(model, conn, 0)) {
            Rented_cars.push_back(model);
            try {
                pqxx::work txn(conn);
                std::string updateQuery = "UPDATE employees SET Rented_cars = ";
                updateQuery += "{";
                for (size_t i = 0; i < Rented_cars.size(); ++i) {
                    updateQuery += "'" + Rented_cars[i] + "'";
                    if (i != Rented_cars.size() - 1) {
                        updateQuery += ",";
                    }
                }
                updateQuery += "} WHERE ID = '" + ID + "'";

                txn.exec(updateQuery);
                txn.commit();
            } catch (const std::exception& e) {
                std::cerr << "Error updating rented cars: " << e.what() << std::endl;
            }
        }
    }

    void rentedCars() override {
        std::cout << "Rented Cars: ";
        for (const auto& car : Rented_cars) {
            std::cout << car << " ";
        }
        std::cout << std::endl;
    }

    void seeRecords() override {
        std::cout << "Records: " << record << std::endl;
    }

    void calculateFine(pqxx::connection& conn) override {
        try {
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
            int totalFine = 0;
            int overdueCarsCount = 0;
            int totalCarsCount = Rented_cars.size();

            for (const auto& carModel : Rented_cars) {
                pqxx::work txn(conn);
                std::string query = "SELECT * FROM cars WHERE model = " + conn.quote(carModel);
                pqxx::result result = txn.exec(query);
                txn.commit();

                if (!result.empty()) {
                    std::string dueDateStr = result[0]["due_date"].as<std::string>();
                    std::tm dueDate = {};
                    std::istringstream ss(dueDateStr);
                    ss >> std::get_time(&dueDate, "%Y-%m-%d %H:%M:%S");

                    auto dueDateTime = std::chrono::system_clock::from_time_t(std::mktime(&dueDate));

                    if (now > dueDateTime) {
                        auto overdueDuration = std::chrono::duration_cast<std::chrono::minutes>(now - dueDateTime);
                        int dailyFine = result[0]["daily_fine"].as<int>();
                        int fine = dailyFine * static_cast<int>(overdueDuration.count());

                        totalFine += fine;
                        overdueCarsCount++;
                    }
                }
            }

            pqxx::work updateTxn(conn);
            Fine_due = totalFine;
            updateTxn.exec("UPDATE customers SET Fine_due = " + std::to_string(totalFine) + " WHERE ID = '" + ID + "'");

            std::string customerRecord = (overdueCarsCount >= totalCarsCount / 2) ? "BAD" : "GOOD";
            record = customerRecord;
            updateTxn.exec("UPDATE customers SET customer_record = '" + customerRecord + "' WHERE ID = '" + ID + "'");
            
            updateTxn.commit();
            
            std::cout << "Fine calculated successfully. Total fine due: " << totalFine << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

};

class Manager : public User {

public:
    pqxx::connection &conn;

    Manager(const std::string& id, const std::string& password, const std::string& name, pqxx::connection& conn)
        : User(name, id, password), conn(conn) {}

    void AddCustomer(const std::string& name, const std::string& id, const std::string& password) {
        pqxx::work txn(conn);
        txn.exec("INSERT INTO customers (name, id, password) VALUES ('" + name + "', '" + id + "', '" + password + "')");
        txn.commit();
    }

    void UpdateCustomer(const std::string& id, const std::string& newName, const std::string& newPassword) {
        pqxx::work txn(conn);
        txn.exec("UPDATE customers SET name = '" + newName + "', password = '" + newPassword + "' WHERE id = '" + id + "'");
        txn.commit();
    }

    void DeleteCustomer(const std::string& id) {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM customers WHERE id = '" + id + "'");
        txn.commit();
    }

    void AddEmployee(const std::string& name, const std::string& id, const std::string& password) {
        pqxx::work txn(conn);
        txn.exec("INSERT INTO employees (name, id, password) VALUES ('" + name + "', '" + id + "', '" + password + "')");
        txn.commit();
    }

    void UpdateEmployee(const std::string& id, const std::string& newName, const std::string& newPassword) {
        pqxx::work txn(conn);
        txn.exec("UPDATE employees SET name = '" + newName + "', password = '" + newPassword + "' WHERE id = '" + id + "'");
        txn.commit();
    }

    void DeleteEmployee(const std::string& id) {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM employees WHERE id = '" + id + "'");
        txn.commit();
    }

    void AddCar(const std::string& model, int condition, const std::string& otherDetails, int dailyCharge, int dailyFine) {
        pqxx::work txn(conn);
        txn.exec("INSERT INTO cars (model, condition, other_details, daily_charge, daily_fine, rented_status) VALUES ('" + model + "', " + std::to_string(condition) + ", '" + otherDetails + "', " + std::to_string(dailyCharge) + ", " + std::to_string(dailyFine) + ", true)");
        txn.commit();
    }

    void UpdateCar(const std::string& model, int condition, const std::string& otherDetails, int dailyCharge, int dailyFine) {
        pqxx::work txn(conn);
        txn.exec("UPDATE cars SET condition = " + std::to_string(condition) + ", other_details = '" + otherDetails + "', daily_charge = " + std::to_string(dailyCharge) + ", daily_fine = " + std::to_string(dailyFine) + " WHERE model = '" + model + "'");
        txn.commit();
    }

    void DeleteCar(const std::string& model) {
        pqxx::work txn(conn);
        txn.exec("DELETE FROM cars WHERE model = '" + model + "'");
        txn.commit();
    }

    void clearDues(const std::string& ID, const std::string& model, int option) {
        try {
            pqxx::work txn(conn);
            std::string tableName = (option == 1) ? "customers" : "employees";
            std::string query = "SELECT * FROM " + tableName + " WHERE ID = " + conn.quote(ID);
            pqxx::result result = txn.exec(query);
            txn.commit();

            if (!result.empty()) {
                std::vector<std::string> rentedCars;
                std::string rentedCarsStr = result[0]["rented_cars"].as<std::string>();
                std::stringstream ss(rentedCarsStr);
                std::string car;
                while (std::getline(ss, car, ',')) {
                    if (car != model) { // Exclude the car to be cleared
                        rentedCars.push_back(car);
                    }
                }

                // Join rentedCars into a comma-separated string
                std::string updatedRentedCars = "";
                for (size_t i = 0; i < rentedCars.size(); ++i) {
                    updatedRentedCars += rentedCars[i];
                    if (i != rentedCars.size() - 1) {
                        updatedRentedCars += ",";
                    }
                }

                // Update rented_cars field in the database
                pqxx::work updateTxn(conn);
                updateTxn.exec("UPDATE " + tableName + " SET rented_cars = '{" + updatedRentedCars + "}' WHERE ID = " + conn.quote(ID));
                updateTxn.commit();

                std::cout << "Dues cleared successfully for user with ID: " << ID << std::endl;
            } else {
                throw std::runtime_error("User not found in the database.");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    // Not used Func, included to avoid manager becoming abstract class
    void viewFine()override{}
    void rentRequest(const std::string& model, pqxx::connection& conn) override{}
    void rentedCars()override{}
    void seeRecords()override{}
    void calculateFine(pqxx::connection& conn) override{}
};

#endif // USER_H