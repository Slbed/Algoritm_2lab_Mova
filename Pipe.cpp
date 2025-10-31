#include "Pipe.h"
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

void Pipe::edit() {
    underRepair = !underRepair;
}

Pipe Pipe::loadFromStream(std::ifstream& in) {
    Pipe pipe;
    in >> pipe.id;
    in.ignore();
    std::getline(in, pipe.name);
    in >> pipe.length >> pipe.diameter >> pipe.underRepair;
    if (pipe.id >= nextId) nextId = pipe.id + 1;
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