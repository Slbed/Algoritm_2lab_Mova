#include "Network.h"
#include "Utils.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <limits>

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
        removeConnectionsByPipeId(id);

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
        removeConnectionsByStationId(id);

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

    Pipe* pipe = getPipeById(pipeId);
    if (pipe) {
        Connection conn(pipeId, csInId, csOutId, diameter);
        conn.weight = pipe->calculateWeight();
        conn.capacity = pipe->calculateCapacity();
        connections.emplace_back(conn);
    }
    else {
        connections.emplace_back(pipeId, csInId, csOutId, diameter);
    }

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

void Network::updateConnectionProperties() {
    for (auto& conn : connections) {
        Pipe* pipe = getPipeById(conn.id);
        if (pipe) {
            conn.weight = pipe->calculateWeight();
            conn.capacity = pipe->calculateCapacity();
        }
    }
}

void Network::rebuildGraph() {
    adjacencyList.clear();
    inDegree.clear();
    updateConnectionProperties();

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

    if ((int)sorted.size() != (int)stations.size()) {
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
                << conn.diameter << std::endl
                << conn.weight << std::endl
                << conn.capacity << std::endl;
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
                double weight, capacity;
                if (!(in >> pipeId >> csInId >> csOutId >> diameter >> weight >> capacity)) {
                    std::cout << "Error reading connection data" << std::endl;
                    break;
                }
                in.ignore();

                if (pipes.find(pipeId) != pipes.end()) {
                    Connection conn(pipeId, csInId, csOutId, diameter);
                    conn.weight = weight;
                    conn.capacity = capacity;
                    connections.emplace_back(conn);
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

void Network::removeConnectionsByPipeId(int pipeId) {
    auto it = std::remove_if(connections.begin(), connections.end(),
        [pipeId](const Connection& conn) {
            return conn.id == pipeId;
        });

    int removedCount = (int)std::distance(it, connections.end());
    connections.erase(it, connections.end());

    if (removedCount > 0) {
        Logger::log("Removed " + std::to_string(removedCount) +
            " connections using pipe ID: " + std::to_string(pipeId));
        std::cout << "Removed " << removedCount << " connection(s) using pipe ID " << pipeId << std::endl;

        rebuildGraph();
    }
}

void Network::removeConnectionsByStationId(int stationId) {
    auto it = std::remove_if(connections.begin(), connections.end(),
        [stationId](const Connection& conn) {
            return conn.csInId == stationId || conn.csOutId == stationId;
        });

    int removedCount = (int)std::distance(it, connections.end());
    connections.erase(it, connections.end());

    if (removedCount > 0) {
        Logger::log("Removed " + std::to_string(removedCount) +
            " connections related to station ID: " + std::to_string(stationId));
        std::cout << "Removed " << removedCount << " connection(s) related to station ID " << stationId << std::endl;

        rebuildGraph();
    }
}

bool Network::bfsForFlow(int source, int sink,
    const std::vector<std::vector<double>>& capacity,
    std::vector<int>& parent) const {
    int n = (int)capacity.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && capacity[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;

                if (v == sink) {
                    return true;
                }
            }
        }
    }

    return false;
}

double Network::calculateMaxFlow(int sourceId, int sinkId) {
    if (stations.empty() || connections.empty()) {
        std::cout << "Error: Network is empty!" << std::endl;
        return 0.0;
    }

    if (!getStationById(sourceId)) {
        std::cout << "Error: Source station with ID " << sourceId << " not found!" << std::endl;
        return 0.0;
    }

    if (!getStationById(sinkId)) {
        std::cout << "Error: Sink station with ID " << sinkId << " not found!" << std::endl;
        return 0.0;
    }

    if (sourceId == sinkId) {
        std::cout << "Error: Source and sink cannot be the same station!" << std::endl;
        return 0.0;
    }

    std::unordered_map<int, int> idToIndex;
    std::unordered_map<int, int> indexToId;
    int index = 0;
    for (const auto& station_pair : stations) {
        idToIndex[station_pair.first] = index;
        indexToId[index] = station_pair.first;
        index++;
    }

    int n = (int)stations.size();
    int sourceIndex = idToIndex[sourceId];
    int sinkIndex = idToIndex[sinkId];
    std::vector<std::vector<double>> capacity(n, std::vector<double>(n, 0.0));

    for (const auto& conn : connections) {
        int u = idToIndex[conn.csInId];
        int v = idToIndex[conn.csOutId];
        capacity[u][v] += conn.capacity;
    }

    // Алгоритм Эдмондса-Карпа (упрощенный фалкерсон)
    std::vector<int> parent(n);
    double maxFlow = 0.0;

    while (bfsForFlow(sourceIndex, sinkIndex, capacity, parent)) {
        double pathFlow = std::numeric_limits<double>::max();
        for (int v = sinkIndex; v != sourceIndex; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, capacity[u][v]);
        }

        for (int v = sinkIndex; v != sourceIndex; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= pathFlow;
            capacity[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    std::cout << "\n=== MAXIMUM FLOW CALCULATION ===" << std::endl;
    std::cout << "Source CS: " << sourceId << std::endl;
    std::cout << "Sink CS: " << sinkId << std::endl;
    std::cout << "Maximum flow: " << maxFlow << " thousand m3/day" << std::endl;

    Logger::log("Calculated max flow from CS " + std::to_string(sourceId) +
        " to CS " + std::to_string(sinkId) + ": " +
        std::to_string(maxFlow) + " thousand m3/day");

    return maxFlow;
}

double Network::dijkstra(int startId, int endId, std::vector<int>& path) {
    if (stations.empty() || connections.empty()) {
        return std::numeric_limits<double>::max();
    }

    std::unordered_map<int, int> idToIndex;
    std::unordered_map<int, int> indexToId;
    int index = 0;
    for (const auto& station_pair : stations) {
        idToIndex[station_pair.first] = index;
        indexToId[index] = station_pair.first;
        index++;
    }

    int n = (int)stations.size();
    int startIndex = idToIndex[startId];
    int endIndex = idToIndex[endId];

    std::vector<double> dist(n, std::numeric_limits<double>::max());
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);

    dist[startIndex] = 0;

    std::vector<std::vector<double>> weight(n, std::vector<double>(n, std::numeric_limits<double>::max()));

    for (const auto& conn : connections) {
        int u = idToIndex[conn.csInId];
        int v = idToIndex[conn.csOutId];
        weight[u][v] = conn.weight;
    }

    // Алгоритм Дейкстры
    for (int i = 0; i < n; ++i) {
        int u = -1;
        double minDist = std::numeric_limits<double>::max();

        for (int j = 0; j < n; ++j) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1 || u == endIndex) {
            break;
        }

        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && weight[u][v] < std::numeric_limits<double>::max()) {
                double newDist = dist[u] + weight[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                }
            }
        }
    }

    if (dist[endIndex] < std::numeric_limits<double>::max()) {
        std::vector<int> reversePath;
        for (int v = endIndex; v != -1; v = prev[v]) {
            reversePath.push_back(indexToId[v]);
        }

        path.clear();
        for (int i = (int)reversePath.size() - 1; i >= 0; --i) {
            path.push_back(reversePath[i]);
        }
    }

    return dist[endIndex];
}

std::vector<int> Network::findShortestPath(int startId, int endId) {
    if (stations.empty() || connections.empty()) {
        std::cout << "Error: Network is empty!" << std::endl;
        return {};
    }

    if (!getStationById(startId)) {
        std::cout << "Error: Start station with ID " << startId << " not found!" << std::endl;
        return {};
    }

    if (!getStationById(endId)) {
        std::cout << "Error: End station with ID " << endId << " not found!" << std::endl;
        return {};
    }

    if (startId == endId) {
        std::cout << "Error: Start and end stations cannot be the same!" << std::endl;
        return { startId };
    }

    std::vector<int> path;
    double totalWeight = dijkstra(startId, endId, path);

    std::cout << "\n=== SHORTEST PATH CALCULATION ===" << std::endl;
    std::cout << "Start CS: " << startId << std::endl;
    std::cout << "End CS: " << endId << std::endl;

    if (totalWeight < std::numeric_limits<double>::max()) {
        std::cout << "Total distance: " << totalWeight << " km" << std::endl;
        std::cout << "Path: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << "CS " << path[i];
            if (i < path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;

        std::cout << "\nPipes on the path:" << std::endl;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];


            for (const auto& conn : connections) {
                if (conn.csInId == from && conn.csOutId == to) {
                    Pipe* pipe = getPipeById(conn.id);
                    if (pipe) {
                        std::cout << "  Pipe " << conn.id << " (CS " << from << " -> CS " << to
                            << "): Length = " << pipe->getLength() << " km, "
                            << "Diameter = " << pipe->getDiameter() << " mm, "
                            << "Status = " << (pipe->isUnderRepair() ? "Under repair" : "Working") << std::endl;
                    }
                    break;
                }
            }
        }
    }
    else {
        std::cout << "No path exists between CS " << startId << " and CS " << endId << std::endl;
        path.clear();
    }

    Logger::log("Calculated shortest path from CS " + std::to_string(startId) +
        " to CS " + std::to_string(endId) + ": distance = " +
        std::to_string(totalWeight) + " km");

    return path;
}