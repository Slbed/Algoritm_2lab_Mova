#include "Network.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <iostream>

void Network::addPipe() {
    Pipe pipe;

    std::cout << "=== Add Pipe ===" << std::endl;
    pipe.setName(Utils::getLine("Enter pipe name: "));
    pipe.setLength(Utils::getPositiveDouble("Enter length (km): "));
    pipe.setDiameter(Utils::getPositiveInt("Enter diameter (mm): "));
    pipe.setUnderRepair(Utils::getBool("Is under repair?"));

    int pipeId = pipe.getId();
    pipes.emplace(pipeId, std::move(pipe));

    Logger::log("Added pipe with ID: " + std::to_string(pipeId));
    std::cout << "Pipe added successfully with ID: " << pipeId << std::endl;
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

    int csId = cs.getId();
    stations.emplace(csId, std::move(cs));

    Logger::log("Added CS with ID: " + std::to_string(csId));
    std::cout << "Compressor Station added successfully with ID: " << csId << std::endl;
}

void Network::viewAllObjects() const {
    if (pipes.empty() && stations.empty()) {
        std::cout << "No objects available." << std::endl;
        return;
    }

    if (!pipes.empty()) {
        std::cout << "\n=== PIPES (" << pipes.size() << ") ===" << std::endl;
        for (const auto& pipe_pair : pipes) {
            std::cout << pipe_pair.second << std::endl;
        }
    }
    else {
        std::cout << "\nNo pipes available." << std::endl;
    }

    if (!stations.empty()) {
        std::cout << "\n=== COMPRESSOR STATIONS (" << stations.size() << ") ===" << std::endl;
        for (const auto& station_pair : stations) {
            std::cout << station_pair.second << std::endl;
        }
    }
    else {
        std::cout << "\nNo compressor stations available." << std::endl;
    }
}

std::vector<int> Network::findPipesByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& pipe_pair : pipes) {
        if (pipe_pair.second.getName().find(name) != std::string::npos) {
            result.push_back(pipe_pair.first);
        }
    }
    return result;
}

std::vector<int> Network::findPipesByStatus(bool underRepair) const {
    std::vector<int> result;
    for (const auto& pipe_pair : pipes) {
        if (pipe_pair.second.isUnderRepair() == underRepair) {
            result.push_back(pipe_pair.first);
        }
    }
    return result;
}

std::vector<int> Network::findStationsByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& station_pair : stations) {
        if (station_pair.second.getName().find(name) != std::string::npos) {
            result.push_back(station_pair.first);
        }
    }
    return result;
}

std::vector<int> Network::findStationsByUnusedPercentage(double minPercent) const {
    std::vector<int> result;
    for (const auto& station_pair : stations) {
        if (station_pair.second.getUnusedPercentage() >= minPercent) {
            result.push_back(station_pair.first);
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
    std::cout << "3. Delete all found pipes" << std::endl;

    int choice = Utils::getIntInRange("Choose editing mode: ", 1, 3);

    int processedCount = 0;
    for (int id : pipeIds) {
        Pipe* pipe = getPipeById(id);
        if (pipe) {
            if (choice == 1) {
                std::cout << "Editing pipe ID: " << id << std::endl;
                std::cout << "Current status: " << (pipe->isUnderRepair() ? "Under repair" : "Working") << std::endl;
                pipe->edit();
                std::cout << "New status: " << (pipe->isUnderRepair() ? "Under repair" : "Working") << std::endl;
                processedCount++;
            }
            else if (choice == 2) {
                pipe->fullEdit();
                processedCount++;
            }
            else if (choice == 3) {
                deletePipe(id);
                processedCount++;
            }
        }
    }

    if (choice == 3) {
        Logger::log("Batch deleted " + std::to_string(processedCount) + " pipes");
        std::cout << "Successfully deleted " << processedCount << " pipes." << std::endl;
    }
    else {
        Logger::log("Batch edited " + std::to_string(processedCount) + " pipes");
        std::cout << "Successfully edited " << processedCount << " pipes." << std::endl;
    }
}

void Network::deletePipe(int id) {
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        pipes.erase(it);
        Logger::log("Deleted pipe with ID: " + std::to_string(id));
        std::cout << "Pipe with ID " << id << " deleted successfully!" << std::endl;
    }
    else {
        std::cout << "Error: Pipe with ID " << id << " not found!" << std::endl;
    }
}

void Network::deleteStation(int id) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        stations.erase(it);
        Logger::log("Deleted CS with ID: " + std::to_string(id));
        std::cout << "Compressor station with ID " << id << " deleted successfully!" << std::endl;
    }
    else {
        std::cout << "Error: Compressor station with ID " << id << " not found!" << std::endl;
    }
}

std::vector<int> Network::findAvailablePipesByDiameter(int diameter) const {
    std::vector<int> result;
    for (const auto& pipe_pair : pipes) {
        const Pipe& pipe = pipe_pair.second;
        if (pipe.getDiameter() == diameter && pipe.isAvailable()) {
            result.push_back(pipe_pair.first);
        }
    }
    return result;
}

void Network::connectStations() {
    std::cout << "\n=== CONNECT STATIONS WITH PIPE ===" << std::endl;

    if (stations.size() < 2) {
        std::cout << "Error: Need at least 2 compressor stations to connect!" << std::endl;
        return;
    }

    int csInId = Utils::getPositiveInt("Enter ID of input CS: ");
    CS* csIn = getStationById(csInId);
    if (!csIn) {
        std::cout << "Error: CS with ID " << csInId << " not found!" << std::endl;
        return;
    }

    int csOutId = Utils::getPositiveInt("Enter ID of output CS: ");
    CS* csOut = getStationById(csOutId);
    if (!csOut) {
        std::cout << "Error: CS with ID " << csOutId << " not found!" << std::endl;
        return;
    }

    if (csInId == csOutId) {
        std::cout << "Error: Cannot connect CS to itself!" << std::endl;
        return;
    }

    std::cout << "Allowed diameters: 500, 700, 1000, 1400 mm" << std::endl;
    int diameter;
    while (true) {
        diameter = Utils::getPositiveInt("Enter pipe diameter (mm): ");
        if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
            break;
        }
        std::cout << "Error: Invalid diameter! Allowed: 500, 700, 1000, 1400 mm" << std::endl;
    }

    std::vector<int> availablePipes = findAvailablePipesByDiameter(diameter);

    int pipeId;
    if (!availablePipes.empty()) {
        std::cout << "Found " << availablePipes.size() << " available pipes with diameter "
            << diameter << " mm" << std::endl;
        std::cout << "Available pipe IDs: ";
        for (int id : availablePipes) {
            std::cout << id << " ";
        }
        std::cout << std::endl;

        pipeId = availablePipes[0];
        Pipe* pipe = getPipeById(pipeId);
        pipe->setInUse(true);

        std::cout << "Using existing pipe ID: " << pipeId << std::endl;
    }
    else {
        std::cout << "No available pipes found. Creating new pipe..." << std::endl;
        Pipe newPipe;
        newPipe.setName(Utils::getLine("Enter pipe name: "));
        newPipe.setLength(Utils::getPositiveDouble("Enter length (km): "));
        newPipe.setDiameter(diameter);
        newPipe.setUnderRepair(false);
        newPipe.setInUse(true);

        pipeId = newPipe.getId();
        pipes.emplace(pipeId, std::move(newPipe));

        std::cout << "Created new pipe ID: " << pipeId << std::endl;
    }

    connections.emplace_back(pipeId, csInId, csOutId, diameter);
    rebuildGraph();

    Logger::log("Connected CS " + std::to_string(csInId) + " -> CS " +
        std::to_string(csOutId) + " using pipe " + std::to_string(pipeId));

    std::cout << "Successfully connected CS " << csInId << " to CS " << csOutId
        << " using pipe " << pipeId << std::endl;
}

void Network::viewNetwork() const {
    if (connections.empty()) {
        std::cout << "Network is empty. No connections." << std::endl;
        return;
    }

    std::cout << "\n=== GAS TRANSPORT NETWORK ===" << std::endl;
    std::cout << "Total connections: " << connections.size() << std::endl;

    for (const auto& conn : connections) {
        std::cout << conn << std::endl;
    }

    std::cout << "\nGraph information:" << std::endl;
    std::cout << "Nodes (CS): " << stations.size() << std::endl;
    std::cout << "Edges (connections): " << connections.size() << std::endl;
}

void Network::rebuildGraph() {
    adjacencyList.clear();
    inDegree.clear();

    for (const auto& station_pair : stations) {
        int stationId = station_pair.first;
        inDegree[stationId] = 0;
    }

    for (const auto& conn : connections) {
        adjacencyList[conn.csInId].push_back(conn.csOutId);
        inDegree[conn.csOutId]++;
    }
}

bool Network::hasCycleDFS(int node, std::unordered_map<int, bool>& visited,
    std::unordered_map<int, bool>& recursionStack) const {
    if (!visited[node]) {
        visited[node] = true;
        recursionStack[node] = true;

        if (adjacencyList.find(node) != adjacencyList.end()) {
            for (int neighbor : adjacencyList.at(node)) {
                if (!visited[neighbor] && hasCycleDFS(neighbor, visited, recursionStack)) {
                    return true;
                }
                else if (recursionStack[neighbor]) {
                    return true;
                }
            }
        }
    }

    recursionStack[node] = false;
    return false;
}

bool Network::isAcyclic() const {
    if (stations.empty()) return true;

    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> recursionStack;

    for (const auto& station_pair : stations) {
        visited[station_pair.first] = false;
        recursionStack[station_pair.first] = false;
    }

    for (const auto& station_pair : stations) {
        int node = station_pair.first;
        if (!visited[node]) {
            if (hasCycleDFS(node, visited, recursionStack)) {
                return false;
            }
        }
    }

    return true;
}

void Network::topologicalSort() {
    rebuildGraph();

    if (!isAcyclic()) {
        std::cout << "Error: Graph contains cycles! Topological sort is impossible." << std::endl;
        return;
    }

    std::cout << "\n=== TOPOLOGICAL SORT ===" << std::endl;

    std::unordered_map<int, int> tempInDegree = inDegree;
    std::vector<int> sorted;
    std::vector<int> queue;

    for (const auto& station_pair : stations) {
        int node = station_pair.first;
        if (tempInDegree[node] == 0) {
            queue.push_back(node);
        }
    }

    while (!queue.empty()) {
        std::sort(queue.begin(), queue.end());
        int node = queue[0];
        queue.erase(queue.begin());
        sorted.push_back(node);

        if (adjacencyList.find(node) != adjacencyList.end()) {
            for (int neighbor : adjacencyList[node]) {
                tempInDegree[neighbor]--;
                if (tempInDegree[neighbor] == 0) {
                    queue.push_back(neighbor);
                }
            }
        }
    }

    if (sorted.size() != stations.size()) {
        std::cout << "Error: Graph contains cycles!" << std::endl;
        return;
    }

    std::cout << "Topological order of stations:" << std::endl;
    for (size_t i = 0; i < sorted.size(); i++) {
        CS* station = getStationById(sorted[i]);
        if (station) {
            std::cout << i + 1 << ". CS " << sorted[i] << " - " << station->getName() << std::endl;
        }
    }

    Logger::log("Performed topological sort. Order: " + std::to_string(sorted.size()) + " stations");
}

void Network::saveToFile(const std::string& filename) const {
    try {
        std::ofstream out(filename);
        if (!out.is_open()) {
            std::cout << "Error: Cannot open file for writing: " << filename << std::endl;
            return;
        }

        out << pipes.size() << std::endl;
        for (const auto& pipe_pair : pipes) {
            pipe_pair.second.saveToStream(out);
        }

        out << stations.size() << std::endl;
        for (const auto& station_pair : stations) {
            station_pair.second.saveToStream(out);
        }

        out << connections.size() << std::endl;
        for (const auto& conn : connections) {
            out << conn.id << std::endl
                << conn.csInId << std::endl
                << conn.csOutId << std::endl
                << conn.diameter << std::endl;
        }

        out.close();
        Logger::log("Saved data to file: " + filename);
        std::cout << "Data successfully saved to: " << filename << std::endl;
        std::cout << "Saved " << pipes.size() << " pipes, "
            << stations.size() << " stations, and "
            << connections.size() << " connections." << std::endl;
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

        pipes.clear();
        stations.clear();
        connections.clear();
        adjacencyList.clear();
        inDegree.clear();

        int pipeCount, stationCount, connectionCount;

        if (!(in >> pipeCount)) {
            std::cout << "Error: Invalid file format" << std::endl;
            return;
        }
        in.ignore();

        int maxPipeId = 0;
        for (int i = 0; i < pipeCount; ++i) {
            if (in.eof()) {
                std::cout << "Error: Unexpected end of file while reading pipes" << std::endl;
                return;
            }
            Pipe pipe = Pipe::loadFromStream(in);
            pipes.emplace(pipe.getId(), std::move(pipe));
            if (pipe.getId() > maxPipeId) {
                maxPipeId = pipe.getId();
            }
        }
        Pipe::setIdCounter(maxPipeId);

        if (!(in >> stationCount)) {
            std::cout << "Error: Invalid file format" << std::endl;
            return;
        }
        in.ignore();

        int maxCSId = 0;
        for (int i = 0; i < stationCount; ++i) {
            if (in.eof()) {
                std::cout << "Error: Unexpected end of file while reading stations" << std::endl;
                return;
            }
            CS station = CS::loadFromStream(in);
            stations.emplace(station.getId(), std::move(station));
            if (station.getId() > maxCSId) {
                maxCSId = station.getId();
            }
        }
        CS::setIdCounter(maxCSId);

        if (!(in >> connectionCount)) {
            connectionCount = 0;
        }
        else {
            in.ignore();
            for (int i = 0; i < connectionCount; ++i) {
                if (in.eof()) {
                    std::cout << "Error: Unexpected end of file while reading connections" << std::endl;
                    return;
                }
                int pipeId, csInId, csOutId, diameter;
                if (!(in >> pipeId >> csInId >> csOutId >> diameter)) {
                    std::cout << "Error reading connection data" << std::endl;
                    break;
                }
                in.ignore();

                if (pipes.find(pipeId) != pipes.end()) {
                    connections.emplace_back(pipeId, csInId, csOutId, diameter);
                    pipes[pipeId].setInUse(true);
                }
            }
        }

        in.close();
        rebuildGraph();

        Logger::log("Loaded data from file: " + filename);
        std::cout << "Data successfully loaded from: " << filename << std::endl;
        std::cout << "Loaded " << pipeCount << " pipes, "
            << stationCount << " stations, and "
            << connectionCount << " connections." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error loading file: " << e.what() << std::endl;
    }
}

Pipe* Network::getPipeById(int id) {
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        return &(it->second);
    }
    return nullptr;
}

CS* Network::getStationById(int id) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        return &(it->second);
    }
    return nullptr;
}