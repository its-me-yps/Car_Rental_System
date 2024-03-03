#ifndef RENT_H
#define RENT_H

#include <string>
#include <pqxx/pqxx>
#include <iostream>

bool Request(const std::string& model, pqxx::connection& conn, int discountOption){
    try {
        pqxx::work txn(conn);
        std::string query = "SELECT * FROM cars WHERE model = " + conn.quote(model);
        pqxx::result result = txn.exec(query);
        txn.commit();

        if (result.empty()) {
            std::cerr << "Error: Car with model " << model << " not found." << std::endl;
            return false;
        }

        int condition = result[0]["condition"].as<int>();
        std::string otherDetails = result[0]["other_details"].as<std::string>();
        int dailyCharge = result[0]["daily_charge"].as<int>();
        int dailyFine = result[0]["daily_fine"].as<int>();
        bool rentedStatus = result[0]["rented_status"].as<bool>();

        if (rentedStatus) {
            std::cout << "Error: Car " << model << " is already rented." << std::endl;
            return false;
        }

        int rentDuration;
        std::cout << "Enter rent duration in minutes: ";
        std::cin >> rentDuration;

        if (discountOption == 0) {
            dailyCharge = static_cast<int>(dailyCharge * 0.85);
        }

        std::cout << "Daily Charge: " << dailyCharge << std::endl;
        std::cout << "Daily Fine: " << dailyFine << std::endl;

        char confirm;
        std::cout << "Confirm your Decision (y/n): ";
        std::cin >> confirm;

        if (confirm != 'y' && confirm != 'Y') {
            std::cout << "Rent operation cancelled." << std::endl;
            return false;
        }
        
        auto now = std::chrono::system_clock::now();
        auto rentEndTime = now + std::chrono::minutes(rentDuration);
        std::time_t rentEndTime_c = std::chrono::system_clock::to_time_t(rentEndTime);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&rentEndTime_c), "%Y-%m-%d %H:%M:%S");
        std::string dueDateStr = ss.str();

        pqxx::work updateTxn(conn);
        updateTxn.exec("UPDATE cars SET due_date = '" + dueDateStr + "' WHERE model = " + conn.quote(model));
        updateTxn.commit();
        
        std::cout << "Car " << model << " rented successfully for " << rentDuration << " minutes." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

#endif // RENT_H