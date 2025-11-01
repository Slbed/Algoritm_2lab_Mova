#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <limits>

class Utils {
public:
    static std::string getLine(const std::string& prompt);
    static int getPositiveInt(const std::string& prompt);
    static double getPositiveDouble(const std::string& prompt);
    static double getNonNegativeDouble(const std::string& prompt);
    static bool getBool(const std::string& prompt);
    static int getIntInRange(const std::string& prompt, int min, int max);
    static int getMenuChoice(const std::string& prompt);
};

class Logger {
private:
    static const std::string LOG_FILE;

public:
    static void log(const std::string& message);
};