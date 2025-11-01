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
    std::cout << "Pipe added successfully with ID: " << pipe.getId() << std::endl;
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
    std::cout << "Compressor Station added successfully with ID: " << cs.getId() << std::endl;
}

void Network::viewAllObjects() const {
    if (pipes.empty() && stations.empty()) {
        std::cout << "No objects available." << std::endl;
        return;
    }

    if (!pipes.empty()) {
        std::cout << "\n=== PIPES (" << pipes.size() << ") ===" << std::endl;
        for (const auto& pipe : pipes) {
            std::cout << pipe << std::endl;
        }
    }
    else {
        std::cout << "\nNo pipes available." << std::endl;
    }

    if (!stations.empty()) {
        std::cout << "\n=== COMPRESSOR STATIONS (" << stations.size() << ") ===" << std::endl;
        for (const auto& station : stations) {
            std::cout << station << std::endl;
        }
    }
    else {
        std::cout << "\nNo compressor stations available." << std::endl;
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
        pipe->fullEdit();
        Logger::log("Fully edited pipe with ID: " + std::to_string(id));
        std::cout << "Pipe updated successfully!" << std::endl;
    }
    else {
        std::cout << "Error: Pipe with ID " << id << " not found!" << std::endl;
    }
}

void Network::editStation(int id) {
    CS* station = getStationById(id);
    if (station) {
        station->fullEdit();
        Logger::log("Fully edited CS with ID: " + std::to_string(id));
        std::cout << "Compressor station updated successfully!" << std::endl;
    }
    else {
        std::cout << "Error: Compressor station with ID " << id << " not found!" << std::endl;
    }
}

void Network::batchEditPipes(const std::vector<int>& pipeIds) {
    if (pipeIds.empty()) {
        std::cout << "No pipes to edit." << std::endl;
        return;
    }

    std::cout << "Batch editing " << pipeIds.size() << " pipes" << std::endl;
    std::cout << "1. Toggle repair status only (fast)" << std::endl;
    std::cout << "2. Full edit for each pipe" << std::endl;

    int choice = Utils::getIntInRange("Choose editing mode: ", 1, 2);

    int editedCount = 0;
    for (int id : pipeIds) {
        Pipe* pipe = getPipeById(id);
        if (pipe) {
            if (choice == 1) {
                std::cout << "Editing pipe ID: " << id << std::endl;
                std::cout << "Current status: " << (pipe->isUnderRepair() ? "Under repair" : "Working") << std::endl;
                pipe->edit();
                std::cout << "New status: " << (pipe->isUnderRepair() ? "Under repair" : "Working") << std::endl;
            }
            else {
                pipe->fullEdit();
            }
            editedCount++;
        }
    }
    Logger::log("Batch edited " + std::to_string(editedCount) + " pipes");
    std::cout << "Successfully edited " << editedCount << " pipes." << std::endl;
}

void Network::deletePipe(int id) {
    auto it = std::find_if(pipes.begin(), pipes.end(),
        [id](const Pipe& pipe) { return pipe.getId() == id; });

    if (it != pipes.end()) {
        pipes.erase(it);

        Pipe::resetNextId();
        for (size_t i = 0; i < pipes.size(); ++i) {
            pipes[i].setId(i + 1);
        }

        Logger::log("Deleted pipe with ID: " + std::to_string(id));
        std::cout << "Pipe with ID " << id << " deleted successfully!" << std::endl;
        std::cout << "IDs have been renumbered." << std::endl;
    }
    else {
        std::cout << "Error: Pipe with ID " << id << " not found!" << std::endl;
    }
}

void Network::deleteStation(int id) {
    auto it = std::find_if(stations.begin(), stations.end(),
        [id](const CS& station) { return station.getId() == id; });

    if (it != stations.end()) {
        stations.erase(it);

        CS::resetNextId();
        for (size_t i = 0; i < stations.size(); ++i) {
            stations[i].setId(i + 1);
        }

        Logger::log("Deleted CS with ID: " + std::to_string(id));
        std::cout << "Compressor station with ID " << id << " deleted successfully!" << std::endl;
        std::cout << "IDs have been renumbered." << std::endl;
    }
    else {
        std::cout << "Error: Compressor station with ID " << id << " not found!" << std::endl;
    }
}

void Network::saveToFile(const std::string& filename) const {
    try {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cout << "Error: Cannot open file for writing: " << filename << std::endl;
            return;
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
        std::cout << "Data successfully saved to: " << filename << std::endl;
        std::cout << "Saved " << pipes.size() << " pipes and " << stations.size() << " compressor stations." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving file: " << e.what() << std::endl;
    }
}

void Network::loadFromFile(const std::string& filename) {
    try {
        std::ifstream in(filename);
        if (!in.is_open()) {
            std::cout << "Error: Cannot open file for reading: " << filename << std::endl;
            return;
        }

        std::vector<Pipe> oldPipes = pipes;
        std::vector<CS> oldStations = stations;

        pipes.clear();
        stations.clear();

        int pipeCount, stationCount;
        in >> pipeCount;
        if (in.fail()) {
            std::cout << "Error: Invalid file format" << std::endl;
            pipes = oldPipes;
            stations = oldStations;
            return;
        }
        in.ignore();

        for (int i = 0; i < pipeCount; ++i) {
            if (in.eof()) {
                std::cout << "Error: Unexpected end of file while reading pipes" << std::endl;
                pipes = oldPipes;
                stations = oldStations;
                return;
            }
            pipes.push_back(Pipe::loadFromStream(in));
        }

        in >> stationCount;
        if (in.fail()) {
            std::cout << "Error: Invalid file format" << std::endl;
            pipes = oldPipes;
            stations = oldStations;
            return;
        }
        in.ignore();

        for (int i = 0; i < stationCount; ++i) {
            if (in.eof()) {
                std::cout << "Error: Unexpected end of file while reading stations" << std::endl;
                pipes = oldPipes;
                stations = oldStations;
                return;
            }
            stations.push_back(CS::loadFromStream(in));
        }

        in.close();

        pipes.insert(pipes.end(), oldPipes.begin(), oldPipes.end());
        stations.insert(stations.end(), oldStations.begin(), oldStations.end());

        Pipe::resetNextId();
        for (size_t i = 0; i < pipes.size(); ++i) {
            pipes[i].setId(i + 1);
        }

        CS::resetNextId();
        for (size_t i = 0; i < stations.size(); ++i) {
            stations[i].setId(i + 1);
        }

        Logger::log("Loaded data from file: " + filename);
        std::cout << "Data successfully loaded from: " << filename << std::endl;
        std::cout << "Loaded " << pipeCount << " pipes and " << stationCount << " compressor stations." << std::endl;
        std::cout << "Total objects now: " << pipes.size() << " pipes and " << stations.size() << " compressor stations." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error loading file: " << e.what() << std::endl;
    }
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