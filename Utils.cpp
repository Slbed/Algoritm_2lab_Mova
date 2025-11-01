#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <cctype>

std::string Utils::getLine(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        size_t start = input.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) {
            std::cout << "Error! Input cannot be empty. Please enter a value: ";
            continue;
        }

        size_t end = input.find_last_not_of(" \t\n\r");
        input = input.substr(start, end - start + 1);

        return input;
    }
}

int Utils::getPositiveInt(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input.empty()) {
            std::cout << "Error! Input cannot be empty. Please enter a positive integer: ";
            continue;
        }

        bool is_valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cout << "Error! Please enter a positive integer (digits only): ";
            continue;
        }

        int value = std::stoi(input);
        if (value <= 0) {
            std::cout << "Error! Please enter a positive integer: ";
            continue;
        }

        return value;
    }
}

double Utils::getPositiveDouble(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input.empty()) {
            std::cout << "Error! Input cannot be empty. Please enter a positive number: ";
            continue;
        }

        try {
            double value = std::stod(input);
            if (value <= 0) {
                std::cout << "Error! Please enter a positive number: ";
                continue;
            }
            return value;
        }
        catch (const std::exception&) {
            std::cout << "Error! Please enter a valid number: ";
            continue;
        }
    }
}

double Utils::getNonNegativeDouble(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input.empty()) {
            std::cout << "Error! Input cannot be empty. Please enter a non-negative number: ";
            continue;
        }

        try {
            double value = std::stod(input);
            if (value < 0) {
                std::cout << "Error! Please enter a non-negative number: ";
                continue;
            }
            return value;
        }
        catch (const std::exception&) {
            std::cout << "Error! Please enter a valid number: ";
            continue;
        }
    }
}

bool Utils::getBool(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (1 - yes, 0 - no): ";
        std::getline(std::cin, input);

        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input.empty()) {
            std::cout << "Error! Input cannot be empty. ";
            continue;
        }

        std::string lower_input = input;
        std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

        if (lower_input == "1" || lower_input == "yes" || lower_input == "y" || lower_input == "да" || lower_input == "д") {
            return true;
        }
        else if (lower_input == "0" || lower_input == "no" || lower_input == "n" || lower_input == "нет" || lower_input == "н") {
            return false;
        }
        else {
            std::cout << "Error! Please enter 1 or 0: ";
        }
    }
}

int Utils::getIntInRange(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        value = getPositiveInt(prompt);
        if (value >= min && value <= max) {
            return value;
        }
        std::cout << "Error! Please enter a number between " << min << " and " << max << ": ";
    }
}

int Utils::getMenuChoice(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input.empty()) {
            std::cout << "Error! Input cannot be empty. Please enter a number from 0 to 11: ";
            continue;
        }

        bool is_valid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                is_valid = false;
                break;
            }
        }

        if (!is_valid) {
            std::cout << "Error! Please enter a valid number from 0 to 11: ";
            continue;
        }

        try {
            int value = std::stoi(input);
            if (value < 0 || value > 11) {
                std::cout << "Error! Please enter a number from 0 to 11: ";
                continue;
            }
            return value;
        }
        catch (const std::exception&) {
            std::cout << "Error! Please enter a valid number from 0 to 11: ";
            continue;
        }
    }
}

const std::string Logger::LOG_FILE = "log.txt";

void Logger::log(const std::string& message) {
    std::ofstream out(LOG_FILE, std::ios::app);
    if (out.is_open()) {
        out << message << std::endl;
        out.close();
    }
}