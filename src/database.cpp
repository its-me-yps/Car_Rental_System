#include <iostream>
#include <pqxx/pqxx>

// Create Db and return driver
pqxx::connection init_Db(const std::string& username, const std::string& password, const std::string& host, const std::string& dbname) {
    try {
        std::string db_uri = "postgresql://" + username + ":" + password + "@" + host + "/postgres";
        pqxx::connection conn(db_uri);

        pqxx::nontransaction txn(conn);
        pqxx::result result = txn.exec("SELECT 1 FROM pg_database WHERE datname = " + txn.quote(dbname));

        if (result.empty()) {
            txn.exec("CREATE DATABASE " + txn.quote_name(dbname));
            txn.commit();
        }

        db_uri = "postgresql://" + username + ":" + password + "@" + host + "/" + dbname;
        pqxx::connection conn_with_db(db_uri);
        
        return conn_with_db;
    } catch (const std::exception& e) {
        std::cerr << "Error connecting to database: " << e.what() << std::endl;
        exit(1);
    }
}

// Create tables
void createTables(pqxx::connection& conn) {
    try {
        pqxx::work txn(conn);

        txn.exec(
            "CREATE TABLE IF NOT EXISTS cars ("
            "Model VARCHAR(50) NOT NULL,"
            "Condition INT,"
            "Other_details TEXT,"
            "Due_Date TIMESTAMP,"
            "rented_Status BOOLEAN,"
            "daily_Charges INT,"
            "daily_Fine INT"
            ");"
        );

        txn.exec(
            "CREATE TABLE IF NOT EXISTS customers ("
            "ID VARCHAR(50) PRIMARY KEY,"
            "Name VARCHAR(50),"
            "Password VARCHAR(50),"
            "Rented_cars TEXT[],"
            "Fine_due INT,"
            "customer_record TEXT,"
            "Other_details TEXT"
            ");"
        );

        txn.exec(
            "CREATE TABLE IF NOT EXISTS employees ("
            "ID VARCHAR(50) PRIMARY KEY,"
            "Name VARCHAR(50),"
            "Password VARCHAR(50),"
            "Rented_cars TEXT[],"
            "Fine_due INT,"
            "employee_record TEXT,"
            "Other_details TEXT"
            ");"
        );

        txn.commit();
        // std::cout << "Tables created successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating tables: " << e.what() << std::endl;
        exit(1);
    }
}