#include "Network.h"
#include "Utils.h"
#include <iostream>

void showMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. Add Pipe" << std::endl;
    std::cout << "2. Add Compressor Station" << std::endl;
    std::cout << "3. View All Objects" << std::endl;
    std::cout << "4. Edit Pipe" << std::endl;
    std::cout << "5. Edit Compressor Station" << std::endl;
    std::cout << "6. Search Pipes" << std::endl;
    std::cout << "7. Search Stations" << std::endl;
    std::cout << "8. Batch Edit Pipes" << std::endl;
    std::cout << "9. Delete Object" << std::endl;
    std::cout << "10. Save to File" << std::endl;
    std::cout << "11. Load from File" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

void searchPipesMenu(Network& network) {
    std::cout << "\n=== SEARCH PIPES ===" << std::endl;
    std::cout << "1. By name" << std::endl;
    std::cout << "2. By repair status" << std::endl;

    int choice = Utils::getIntInRange("Choose option: ", 1, 2);

    std::vector<int> results;
    if (choice == 1) {
        std::string name = Utils::getLine("Enter name to search: ");
        results = network.findPipesByName(name);
    }
    else {
        bool status = Utils::getBool("Search pipes under repair?");
        results = network.findPipesByStatus(status);
    }

    std::cout << "Found " << results.size() << " pipes" << std::endl;
    for (int id : results) {
        std::cout << "Pipe ID: " << id << std::endl;
    }
}

void searchStationsMenu(Network& network) {
    std::cout << "\n=== SEARCH STATIONS ===" << std::endl;
    std::cout << "1. By name" << std::endl;
    std::cout << "2. By unused percentage" << std::endl;

    int choice = Utils::getIntInRange("Choose option: ", 1, 2);

    std::vector<int> results;
    if (choice == 1) {
        std::string name = Utils::getLine("Enter name to search: ");
        results = network.findStationsByName(name);
    }
    else {
        double percent = Utils::getNonNegativeDouble("Enter minimum unused percentage: ");
        results = network.findStationsByUnusedPercentage(percent);
    }

    std::cout << "Found " << results.size() << " stations" << std::endl;
    for (int id : results) {
        std::cout << "CS ID: " << id << std::endl;
    }
}

void batchEditMenu(Network& network) {
    std::cout << "\n=== BATCH EDIT PIPES ===" << std::endl;
    std::cout << "1. Edit all pipes under repair" << std::endl;
    std::cout << "2. Edit pipes by name" << std::endl;

    int choice = Utils::getIntInRange("Choose option: ", 1, 2);

    std::vector<int> pipesToEdit;
    if (choice == 1) {
        pipesToEdit = network.findPipesByStatus(true);
    }
    else {
        std::string name = Utils::getLine("Enter pipe name: ");
        pipesToEdit = network.findPipesByName(name);
    }

    if (pipesToEdit.empty()) {
        std::cout << "No pipes found!" << std::endl;
        return;
    }

    std::cout << "Found " << pipesToEdit.size() << " pipes" << std::endl;
    if (Utils::getBool("Edit all found pipes?")) {
        network.batchEditPipes(pipesToEdit);
        std::cout << "Batch edit completed!" << std::endl;
    }
}

void deleteObjectMenu(Network& network) {
    std::cout << "\n=== DELETE OBJECT ===" << std::endl;
    std::cout << "1. Delete Pipe" << std::endl;
    std::cout << "2. Delete Station" << std::endl;

    int choice = Utils::getIntInRange("Choose option: ", 1, 2);
    int id = Utils::getPositiveInt("Enter ID to delete: ");

    if (choice == 1) {
        network.deletePipe(id);
    }
    else {
        network.deleteStation(id);
    }
    std::cout << "Object deleted!" << std::endl;
}

int main() {
    std::cout << "=== Pipeline Control System ===" << std::endl;
    Network network;

    while (true) {
        showMenu();
        int choice = Utils::getMenuChoice("Choose option: ");

        switch (choice) {
        case 1:
            network.addPipe();
            break;
        case 2:
            network.addCS();
            break;
        case 3:
            network.viewAllObjects();
            break;
        case 4: {
            int id = Utils::getPositiveInt("Enter pipe ID to edit: ");
            network.editPipe(id);
            break;
        }
        case 5: {
            int id = Utils::getPositiveInt("Enter station ID to edit: ");
            network.editStation(id);
            break;
        }
        case 6:
            searchPipesMenu(network);
            break;
        case 7:
            searchStationsMenu(network);
            break;
        case 8:
            batchEditMenu(network);
            break;
        case 9:
            deleteObjectMenu(network);
            break;
        case 10: {
            std::string filename = Utils::getLine("Enter filename to save: ");
            network.saveToFile(filename);
            break;
        }
        case 11: {
            std::string filename = Utils::getLine("Enter filename to load: ");
            network.loadFromFile(filename);
            break;
        }
        case 0:
            std::cout << "Goodbye!" << std::endl;
            return 0;
        }
    }

    return 0;
}