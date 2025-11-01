#include "Utils.h"
#include <fstream>

std::string Utils::getLine(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int Utils::getPositiveInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "Error! Please enter a positive integer: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cin.ignore();
            return value;
        }
    }
}

double Utils::getPositiveDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cout << "Error! Please enter a positive number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cin.ignore();
            return value;
        }
    }
}

bool Utils::getBool(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (1 - yes, 0 - no): ";
        std::cin >> input;
        if (input == "1" || input == "yes") {
            std::cin.ignore();
            return true;
        }
        else if (input == "0" || input == "no") {
            std::cin.ignore();
            return false;
        }
        else {
            std::cout << "Error! Please enter 1 or 0: ";
            std::cin.ignore();
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

const std::string Logger::LOG_FILE = "log.txt";

void Logger::log(const std::string& message) {
    std::ofstream out(LOG_FILE, std::ios::app);
    if (out.is_open()) {
        out << message << std::endl;
        out.close();
    }
}