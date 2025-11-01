#pragma once
#include <iostream>
#include <string>

class CS {
private:
    static int nextId;
    int id;
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    std::string efficiency;

public:
    CS();

    int getId() const;
    std::string getName() const;
    int getTotalWorkshops() const;
    int getWorkingWorkshops() const;
    std::string getEfficiency() const;
    double getUnusedPercentage() const;

    void setName(const std::string& name);
    void setTotalWorkshops(int total);
    void setWorkingWorkshops(int working);
    void setEfficiency(const std::string& efficiency);
    void setId(int newId);

    void edit();
    void fullEdit();

    static CS loadFromStream(std::ifstream& in);
    void saveToStream(std::ofstream& out) const;

    static void resetNextId();

    friend std::ostream& operator<<(std::ostream& os, const CS& cs);
};