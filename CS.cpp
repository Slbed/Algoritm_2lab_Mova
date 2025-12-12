#include "CS.h"
#include "Utils.h"
#include <fstream>

int CS::id_counter = 1;


CS::CS()
    : id(id_counter++), name(""), totalWorkshops(0), workingWorkshops(0), efficiency("") {
}


CS::CS(int forcedId)
    : id(forcedId), name(""), totalWorkshops(0), workingWorkshops(0), efficiency("") {
    if (forcedId >= id_counter) {
        id_counter = forcedId + 1;
    }
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

void CS::resetIdCounter() {
    id_counter = 1;
}

void CS::setIdCounter(int new_counter) {
    if (new_counter >= id_counter) {
        id_counter = new_counter + 1;
    }
}

void CS::edit() {
    if (workingWorkshops < totalWorkshops) {
        workingWorkshops++;
    }
}

void CS::fullEdit() {
    std::cout << "=== Editing Compressor Station ID: " << id << " ===" << std::endl;

    std::cout << "Current name: " << name << std::endl;
    if (Utils::getBool("Change name?")) {
        setName(Utils::getLine("Enter new station name: "));
        std::cout << "Name updated to: " << name << std::endl;
    }

    std::cout << "Current total workshops: " << totalWorkshops << std::endl;
    if (Utils::getBool("Change total workshops?")) {
        int newTotal = Utils::getPositiveInt("Enter new total workshops: ");
        if (newTotal < workingWorkshops) {
            std::cout << "Warning! New total workshops (" << newTotal
                << ") is less than current working workshops (" << workingWorkshops
                << "). Setting working workshops to " << newTotal << std::endl;
            workingWorkshops = newTotal;
        }
        setTotalWorkshops(newTotal);
        std::cout << "Total workshops updated to: " << totalWorkshops << std::endl;
    }

    std::cout << "Current working workshops: " << workingWorkshops << std::endl;
    if (Utils::getBool("Change working workshops?")) {
        int newWorking;
        do {
            newWorking = Utils::getPositiveInt("Enter new working workshops: ");
            if (newWorking > totalWorkshops) {
                std::cout << "Error! Working workshops cannot exceed total workshops ("
                    << totalWorkshops << ")" << std::endl;
            }
        } while (newWorking > totalWorkshops);
        setWorkingWorkshops(newWorking);
        std::cout << "Working workshops updated to: " << workingWorkshops << std::endl;
        std::cout << "Unused workshops: " << (totalWorkshops - workingWorkshops)
            << " (" << getUnusedPercentage() << "%)" << std::endl;
    }

    std::cout << "Current efficiency class: " << efficiency << std::endl;
    if (Utils::getBool("Change efficiency class?")) {
        setEfficiency(Utils::getLine("Enter new efficiency class: "));
        std::cout << "Efficiency class updated to: " << efficiency << std::endl;
    }

    std::cout << "Compressor station editing completed!" << std::endl;
}

CS CS::loadFromStream(std::ifstream& in) {
    int loadedId;
    std::string loadedName;
    int loadedTotalWorkshops;
    int loadedWorkingWorkshops;
    std::string loadedEfficiency;

    in >> loadedId;
    in.ignore();
    std::getline(in, loadedName);
    in >> loadedTotalWorkshops >> loadedWorkingWorkshops;
    in.ignore();
    std::getline(in, loadedEfficiency);

    CS cs(loadedId);
    cs.setName(loadedName);
    cs.setTotalWorkshops(loadedTotalWorkshops);
    cs.setWorkingWorkshops(loadedWorkingWorkshops);
    cs.setEfficiency(loadedEfficiency);

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