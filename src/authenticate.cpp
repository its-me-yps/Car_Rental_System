#include <iostream>
#include <string>
#include <pqxx/pqxx>

void authenticateUser(pqxx::connection& conn, int type, const std::string& ID, const std::string& password) {
    try {
        pqxx::work txn(conn);
        std::string query;

        switch (type) {
            case 1:
                query = "SELECT COUNT(*) FROM customers WHERE id = '" + ID + "' AND password = '" + password + "'";
                break;
            case 2:
                query = "SELECT COUNT(*) FROM employees WHERE id = '" + ID + "' AND password = '" + password + "'";
                break;
            default:
                throw std::runtime_error("Error: Invalid user type.");
        }

        if (type != 3) {
            pqxx::result result = txn.exec(query);
            int count = result.at(0).at(0).as<int>();
            if (count != 1) {
                throw std::runtime_error("Wrong Credentials, Login Failed");
            }
        }
    } catch (const std::exception& e) {
        throw;
    }
}
