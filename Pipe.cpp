#include "Pipe.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <cmath>

int Pipe::id_counter = 1;

Pipe::Pipe()
    : id(id_counter++), name(""), length(0), diameter(0), underRepair(false), inUse(false) {
}

Pipe::Pipe(int forcedId)
    : id(forcedId), name(""), length(0), diameter(0), underRepair(false), inUse(false) {
    if (forcedId >= id_counter) {
        id_counter = forcedId + 1;
    }
}

int Pipe::getId() const { return id; }
std::string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isUnderRepair() const { return underRepair; }
bool Pipe::isInUse() const { return inUse; }
bool Pipe::isAvailable() const { return !inUse && !underRepair; }

void Pipe::setName(const std::string& name) { this->name = name; }
void Pipe::setLength(double length) { this->length = length; }
void Pipe::setDiameter(int diameter) { this->diameter = diameter; }
void Pipe::setUnderRepair(bool status) { this->underRepair = status; }
void Pipe::setInUse(bool status) { this->inUse = status; }

double Pipe::calculateCapacity() const {
    if (underRepair) {
        return 0.0;
    }

    double d_m = diameter / 1000.0; 
    double l_m = length * 1000.0;   


    if (l_m < 0.001) {
        return 0.0;
    }

    double capacity = std::sqrt(std::pow(d_m, 5) / l_m);


    double correction_factor = 1000.0;

    return capacity * correction_factor;
}

double Pipe::calculateWeight() const {
    if (underRepair) {
        return std::numeric_limits<double>::max();
    }
    return length;
}

void Pipe::resetIdCounter() {
    id_counter = 1;
}

void Pipe::setIdCounter(int new_counter) {
    if (new_counter >= id_counter) {
        id_counter = new_counter + 1;
    }
}

void Pipe::edit() {
    underRepair = !underRepair;
}

void Pipe::fullEdit() {
    std::cout << "=== Editing Pipe ID: " << id << " ===" << std::endl;

    std::cout << "Current name: " << name << std::endl;
    if (Utils::getBool("Change name?")) {
        setName(Utils::getLine("Enter new pipe name: "));
        std::cout << "Name updated to: " << name << std::endl;
    }

    std::cout << "Current length: " << length << " km" << std::endl;
    if (Utils::getBool("Change length?")) {
        setLength(Utils::getPositiveDouble("Enter new length (km): "));
        std::cout << "Length updated to: " << length << " km" << std::endl;
    }

    std::cout << "Current diameter: " << diameter << " mm" << std::endl;
    if (Utils::getBool("Change diameter?")) {
        setDiameter(Utils::getPositiveInt("Enter new diameter (mm): "));
        std::cout << "Diameter updated to: " << diameter << " mm" << std::endl;
    }

    std::cout << "Current repair status: " << (underRepair ? "Under repair" : "Working") << std::endl;
    if (Utils::getBool("Change repair status?")) {
        setUnderRepair(Utils::getBool("Is under repair?"));
        std::cout << "Repair status updated to: " << (underRepair ? "Under repair" : "Working") << std::endl;
    }

    std::cout << "Current usage status: " << (inUse ? "In use (connected)" : "Not in use") << std::endl;
    if (Utils::getBool("Change usage status? (Warning: affects connections)")) {
        setInUse(Utils::getBool("Is in use?"));
        std::cout << "Usage status updated to: " << (inUse ? "In use" : "Not in use") << std::endl;
        std::cout << "Warning: Manually changing usage status may cause inconsistencies in connections!" << std::endl;
    }

    std::cout << "Pipe editing completed!" << std::endl;
}

Pipe Pipe::loadFromStream(std::ifstream& in) {
    int loadedId;
    std::string loadedName;
    double loadedLength;
    int loadedDiameter;
    bool loadedUnderRepair;
    bool loadedInUse;

    in >> loadedId;
    in.ignore();
    std::getline(in, loadedName);
    in >> loadedLength >> loadedDiameter >> loadedUnderRepair >> loadedInUse;
    in.ignore();

    Pipe pipe(loadedId);
    pipe.setName(loadedName);
    pipe.setLength(loadedLength);
    pipe.setDiameter(loadedDiameter);
    pipe.setUnderRepair(loadedUnderRepair);
    pipe.setInUse(loadedInUse);

    return pipe;
}

void Pipe::saveToStream(std::ofstream& out) const {
    out << id << std::endl
        << name << std::endl
        << length << std::endl
        << diameter << std::endl
        << underRepair << std::endl
        << inUse << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Pipe& pipe) {
    os << "=== Pipe ID: " << pipe.id << " ===" << std::endl
        << "Name: " << pipe.name << std::endl
        << "Length: " << pipe.length << " km" << std::endl
        << "Diameter: " << pipe.diameter << " mm" << std::endl
        << "Capacity: " << pipe.calculateCapacity() << " thousand m3/day" << std::endl
        << "Weight: " << pipe.calculateWeight() << std::endl
        << "Under repair: " << (pipe.underRepair ? "Yes" : "No") << std::endl
        << "In use: " << (pipe.inUse ? "Yes (connected)" : "No") << std::endl
        << "Available for new connections: " << (pipe.isAvailable() ? "Yes" : "No") << std::endl;
    return os;
}