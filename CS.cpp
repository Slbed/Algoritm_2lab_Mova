#include "CS.h"
#include <fstream>

int CS::nextId = 1;

CS::CS()
    : id(nextId++), name(""), totalWorkshops(0), workingWorkshops(0), efficiency("") {
}

int CS::getId() const { return id; }
std::string CS::getName() const { return name; }
int CS::getTotalWorkshops() const { return totalWorkshops; }
int CS::getWorkingWorkshops() const { return workingWorkshops; }
std::string CS::getEfficiency() const { return efficiency; }

double CS::getUnusedPercentage() const {
    if (totalWorkshops == 0) return 0;
    return ((totalWorkshops - workingWorkshops) * 100.0) / totalWorkshops;
}

void CS::setName(const std::string& name) { this->name = name; }
void CS::setTotalWorkshops(int total) { this->totalWorkshops = total; }
void CS::setWorkingWorkshops(int working) { this->workingWorkshops = working; }
void CS::setEfficiency(const std::string& efficiency) { this->efficiency = efficiency; }

void CS::edit() {
    if (workingWorkshops < totalWorkshops) {
        workingWorkshops++;
    }
}

CS CS::loadFromStream(std::ifstream& in) {
    CS cs;
    in >> cs.id;
    in.ignore();
    std::getline(in, cs.name);
    in >> cs.totalWorkshops >> cs.workingWorkshops;
    in.ignore();
    std::getline(in, cs.efficiency);
    if (cs.id >= nextId) nextId = cs.id + 1;
    return cs;
}

void CS::saveToStream(std::ofstream& out) const {
    out << id << std::endl
        << name << std::endl
        << totalWorkshops << std::endl
        << workingWorkshops << std::endl
        << efficiency << std::endl;
}

std::ostream& operator<<(std::ostream& os, const CS& cs) {
    os << "=== CS ID: " << cs.id << " ===" << std::endl
        << "Name: " << cs.name << std::endl
        << "Workshops: " << cs.workingWorkshops << "/" << cs.totalWorkshops << std::endl
        << "Unused: " << cs.getUnusedPercentage() << "%" << std::endl
        << "Efficiency: " << cs.efficiency << std::endl;
    return os;
}