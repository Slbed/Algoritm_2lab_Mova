#include "Network.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>

void Network::addPipe() {
    Pipe pipe;

    std::cout << "=== Add Pipe ===" << std::endl;
    pipe.setName(Utils::getLine("Enter pipe name: "));
    pipe.setLength(Utils::getPositiveDouble("Enter length (km): "));
    pipe.setDiameter(Utils::getPositiveInt("Enter diameter (mm): "));
    pipe.setUnderRepair(Utils::getBool("Is under repair?"));

    pipes.push_back(pipe);
    Logger::log("Added pipe with ID: " + std::to_string(pipe.getId()));
}

void Network::addCS() {
    CS cs;

    std::cout << "=== Add Compressor Station ===" << std::endl;
    cs.setName(Utils::getLine("Enter station name: "));
    cs.setTotalWorkshops(Utils::getPositiveInt("Enter total workshops: "));

    int working;
    do {
        working = Utils::getPositiveInt("Enter working workshops: ");
        if (working > cs.getTotalWorkshops()) {
            std::cout << "Error! Working workshops cannot exceed total workshops ("
                << cs.getTotalWorkshops() << ")" << std::endl;
        }
    } while (working > cs.getTotalWorkshops());

    cs.setWorkingWorkshops(working);
    cs.setEfficiency(Utils::getLine("Enter efficiency class: "));

    stations.push_back(cs);
    Logger::log("Added CS with ID: " + std::to_string(cs.getId()));
}

void Network::viewAllObjects() const {
    std::cout << "\n=== PIPES ===" << std::endl;
    for (const auto& pipe : pipes) {
        std::cout << pipe << std::endl;
    }

    std::cout << "\n=== COMPRESSOR STATIONS ===" << std::endl;
    for (const auto& station : stations) {
        std::cout << station << std::endl;
    }
}

std::vector<int> Network::findPipesByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& pipe : pipes) {
        if (pipe.getName().find(name) != std::string::npos) {
            result.push_back(pipe.getId());
        }
    }
    return result;
}

std::vector<int> Network::findPipesByStatus(bool underRepair) const {
    std::vector<int> result;
    for (const auto& pipe : pipes) {
        if (pipe.isUnderRepair() == underRepair) {
            result.push_back(pipe.getId());
        }
    }
    return result;
}

std::vector<int> Network::findStationsByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& station : stations) {
        if (station.getName().find(name) != std::string::npos) {
            result.push_back(station.getId());
        }
    }
    return result;
}

std::vector<int> Network::findStationsByUnusedPercentage(double minPercent) const {
    std::vector<int> result;
    for (const auto& station : stations) {
        if (station.getUnusedPercentage() >= minPercent) {
            result.push_back(station.getId());
        }
    }
    return result;
}

void Network::editPipe(int id) {
    Pipe* pipe = getPipeById(id);
    if (pipe) {
        pipe->edit();
        Logger::log("Edited pipe with ID: " + std::to_string(id));
    }
    else {
        std::cout << "Pipe with ID " << id << " not found!" << std::endl;
    }
}

void Network::editStation(int id) {
    CS* station = getStationById(id);
    if (station) {
        station->edit();
        Logger::log("Edited CS with ID: " + std::to_string(id));
    }
    else {
        std::cout << "CS with ID " << id << " not found!" << std::endl;
    }
}

void Network::batchEditPipes(const std::vector<int>& pipeIds) {
    for (int id : pipeIds) {
        editPipe(id);
    }
    Logger::log("Batch edited " + std::to_string(pipeIds.size()) + " pipes");
}

void Network::deletePipe(int id) {
    deleteObject(pipes, id);
    Logger::log("Deleted pipe with ID: " + std::to_string(id));
}

void Network::deleteStation(int id) {
    deleteObject(stations, id);
    Logger::log("Deleted CS with ID: " + std::to_string(id));
}

void Network::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    out << pipes.size() << std::endl;
    for (const auto& pipe : pipes) {
        pipe.saveToStream(out);
    }

    out << stations.size() << std::endl;
    for (const auto& station : stations) {
        station.saveToStream(out);
    }

    out.close();
    Logger::log("Saved data to file: " + filename);
}

void Network::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    pipes.clear();
    stations.clear();

    int pipeCount, stationCount;
    in >> pipeCount;
    in.ignore();

    for (int i = 0; i < pipeCount; ++i) {
        pipes.push_back(Pipe::loadFromStream(in));
    }

    in >> stationCount;
    in.ignore();

    for (int i = 0; i < stationCount; ++i) {
        stations.push_back(CS::loadFromStream(in));
    }

    in.close();
    Logger::log("Loaded data from file: " + filename);
}

Pipe* Network::getPipeById(int id) {
    for (auto& pipe : pipes) {
        if (pipe.getId() == id) {
            return &pipe;
        }
    }
    return nullptr;
}

CS* Network::getStationById(int id) {
    for (auto& station : stations) {
        if (station.getId() == id) {
            return &station;
        }
    }
    return nullptr;
}

template<typename T>
void Network::deleteObject(std::vector<T>& objects, int id) {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [id](const T& obj) { return obj.getId() == id; }),
        objects.end()
    );
}