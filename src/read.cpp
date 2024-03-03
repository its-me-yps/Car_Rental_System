#include <fstream>
#include <sstream>
#include <iostream>

struct Credentials {
    std::string username;
    std::string password;
};

Credentials readSecretFile(const std::string& filename, const std::string field1, const std::string field2) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open secret file.");
    }

    std::string line;
    std::string user, password;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            if (key == field1) {
                user = value;
            } else if (key == field2) {
                password = value;
            }
        }
    }
    return {user, password};
}