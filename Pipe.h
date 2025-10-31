#pragma once
#include <iostream>
#include <string>

class Pipe {
private:
    static int nextId;
    int id;
    std::string name;
    double length;
    int diameter;
    bool underRepair;

public:
    Pipe();

    int getId() const;
    std::string getName() const;
    double getLength() const;
    int getDiameter() const;
    bool isUnderRepair() const;

    void setName(const std::string& name);
    void setLength(double length);
    void setDiameter(int diameter);
    void setUnderRepair(bool status);

    void edit();
    static Pipe loadFromStream(std::ifstream& in);
    void saveToStream(std::ofstream& out) const;

    friend std::ostream& operator<<(std::ostream& os, const Pipe& pipe);
};