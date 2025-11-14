#include "Pipe.h"
#include "Utils.h"
#include <fstream>

int Pipe::nextId = 1;

Pipe::Pipe() : id(nextId++), name(""), length(0), diameter(0), underRepair(false) {}

int Pipe::getId() const { return id; }
std::string Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::isUnderRepair() const { return underRepair; }

void Pipe::setName(const std::string& name) { this->name = name; }
void Pipe::setLength(double length) { this->length = length; }
void Pipe::setDiameter(int diameter) { this->diameter = diameter; }
void Pipe::setUnderRepair(bool status) { this->underRepair = status; }
void Pipe::setId(int newId) {
    this->id = newId;
    if (newId >= nextId) {
        nextId = newId + 1;
    }
}

void Pipe::resetNextId() {
    nextId = 1;
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

    std::cout << "Pipe editing completed!" << std::endl;
}

Pipe Pipe::loadFromStream(std::ifstream& in) {
    Pipe pipe;
    in >> pipe.id;
    in.ignore();
    std::getline(in, pipe.name);
    in >> pipe.length >> pipe.diameter >> pipe.underRepair;
    if (pipe.id >= nextId) {
        nextId = pipe.id + 1;
    }
    return pipe;
}

void Pipe::saveToStream(std::ofstream& out) const {
    out << id << std::endl
        << name << std::endl
        << length << std::endl
        << diameter << std::endl
        << underRepair << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Pipe& pipe) {
    os << "=== Pipe ID: " << pipe.id << " ===" << std::endl
        << "Name: " << pipe.name << std::endl
        << "Length: " << pipe.length << " km" << std::endl
        << "Diameter: " << pipe.diameter << " mm" << std::endl
        << "Under repair: " << (pipe.underRepair ? "Yes" : "No") << std::endl;
    return os;
}