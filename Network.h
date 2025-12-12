#pragma once
#include "Pipe.h"
#include "CS.h"
#include "Connection.h"
#include <unordered_map>
#include <vector>
#include <list>

class Network {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, CS> stations;
    std::vector<Connection> connections;  

    std::unordered_map<int, std::vector<int>> adjacencyList;  
    std::unordered_map<int, int> inDegree;                   

public:
    void addPipe();
    void addCS();
    void viewAllObjects() const;


    void connectStations();                   
    void viewNetwork() const;               
    void topologicalSort();                    
    bool isAcyclic() const;                    
    void rebuildGraph();                     

    std::vector<int> findPipesByName(const std::string& name) const;
    std::vector<int> findPipesByStatus(bool underRepair) const;
    std::vector<int> findStationsByName(const std::string& name) const;
    std::vector<int> findStationsByUnusedPercentage(double minPercent) const;


    std::vector<int> findAvailablePipesByDiameter(int diameter) const;  

    void editPipe(int id);
    void editStation(int id);
    void batchEditPipes(const std::vector<int>& pipeIds);

    void deletePipe(int id);
    void deleteStation(int id);

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    Pipe* getPipeById(int id);
    CS* getStationById(int id);

private:

    bool hasCycleDFS(int node, std::unordered_map<int, bool>& visited,
        std::unordered_map<int, bool>& recursionStack) const;
};