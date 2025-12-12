#pragma once
#include <iostream>
#include <string>

class CS {
private:
    int id;                    
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    std::string efficiency;

public:
    static int id_counter;      

    CS();
    explicit CS(int forcedId);


    CS& operator=(const CS&) = delete;


    CS(const CS& other) = default;

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

    void edit();
    void fullEdit();

    static CS loadFromStream(std::ifstream& in);
    void saveToStream(std::ofstream& out) const;

    static void resetIdCounter();
    static void setIdCounter(int new_counter);

    friend std::ostream& operator<<(std::ostream& os, const CS& cs);
};