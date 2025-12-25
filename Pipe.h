#pragma once
#include <iostream>
#include <string>
#include <limits>

class Pipe {
private:
    int id;
    std::string name;
    double length;       
    int diameter;          
    bool underRepair;
    bool inUse;

public:
    static int id_counter;

    Pipe();
    explicit Pipe(int forcedId);


    Pipe& operator=(const Pipe&) = delete;

    Pipe(const Pipe& other) = default;

    int getId() const;
    std::string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isUnderRepair() const;
    bool isInUse() const;
    bool isAvailable() const;


    void setName(const std::string& name);
    void setLength(double length);
    void setDiameter(int diameter);
    void setUnderRepair(bool status);
    void setInUse(bool status);


    double calculateCapacity() const;  
    double calculateWeight() const;   

    
    void edit();
    void fullEdit();


    static Pipe loadFromStream(std::ifstream& in);
    void saveToStream(std::ofstream& out) const;

    static void resetIdCounter();
    static void setIdCounter(int new_counter);

    friend std::ostream& operator<<(std::ostream& os, const Pipe& pipe);
};