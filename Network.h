#pragma once
#include "Pipe.h"
#include "CS.h"
#include <vector>
#include <functional>

class Network {
private:
    std::vector<Pipe> pipes;
    std::vector<CS> stations;

public:
    void addPipe();
    void addCS();

    void viewAllObjects() const;

    std::vector<int> findPipesByName(const std::string& name) const;
    std::vector<int> findPipesByStatus(bool underRepair) const;
    std::vector<int> findStationsByName(const std::string& name) const;
    std::vector<int> findStationsByUnusedPercentage(double minPercent) const;

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
    template<typename T>
    void deleteObject(std::vector<T>& objects, int id);
};