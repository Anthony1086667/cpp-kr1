#include "Errors.hpp"
#include "Fleet.hpp"
#include "Things.hpp"
#include "Transport.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

std::string readWord(std::istringstream& in) {
    std::string word;
    if (!(in >> word)) {
        throw InvalidArgument("missing argument");
    }
    return word;
}

int readInt(std::istringstream& in) {
    int value = 0;
    if (!(in >> value)) {
        throw InvalidArgument("expected integer");
    }
    return value;
}

double readDouble(std::istringstream& in) {
    double value = 0.0;
    if (!(in >> value)) {
        throw InvalidArgument("expected number");
    }
    return value;
}

void handleAdd(Fleet& fleet, std::istringstream& in) {
    std::string kind = readWord(in);
    std::string id = readWord(in);
    std::string name = readWord(in);

    if (kind == "scooter") {
        double energy = readDouble(in);
        int score = readInt(in);
        fleet.addWithInspection(std::make_unique<Scooter>(id, name, score, energy));
    } else if (kind == "ebike") {
        double energy = readDouble(in);
        int score = readInt(in);
        fleet.addWithInspection(std::make_unique<Ebike>(id, name, score, energy));
    } else if (kind == "bike") {
        int score = readInt(in);
        fleet.addWithInspection(std::make_unique<Bike>(id, name, score));
    } else if (kind == "robot") {
        double energy = readDouble(in);
        int capacity = readInt(in);
        fleet.addWithInspection(std::make_unique<Robot>(id, name, energy, capacity));
    } else if (kind == "helmet") {
        fleet.add(std::make_unique<Helmet>(id, name));
    } else if (kind == "dock") {
        double energy = readDouble(in);
        fleet.add(std::make_unique<Dock>(id, name, energy));
    } else if (kind == "lock") {
        double energy = readDouble(in);
        std::string targetId = readWord(in);
        std::shared_ptr<InventoryItem> target = fleet.find(targetId);
        if (target && !target->canBeLocked()){
            throw InvalidArgument("lock can be attached only to transport");
        }
        fleet.add(std::make_unique<Lock>(id, name, energy, target));
    } else {
        throw InvalidArgument("unknown kind " + kind);
    }
}

bool handleCommand(Fleet& fleet, const std::string& line) {
    std::istringstream in(line);
    std::string command;
    if (!(in >> command)) {
        return true;
    }

    if (command == "quit") {
        return false;
    } else if (command == "add") {
        handleAdd(fleet, in);
    } else if (command == "remove") {
        fleet.remove(readWord(in));
    } else if (command == "list") {
        fleet.printList(std::cout);
    } else if (command == "novice") {
        fleet.printNovice(std::cout);
    } else if (command == "energy") {
        std::cout << std::fixed << std::setprecision(1)
                  << fleet.totalEnergy() << "\n";
    } else if (command == "focus") {
        fleet.setFocus(readWord(in));
    } else if (command == "show_focus") {
        fleet.printFocus(std::cout);
    } else if (command == "info") {
        fleet.printInfo(readWord(in), std::cout);
    } else if (command == "rejected") {
        fleet.printRejected(std::cout);
    } else {
        throw InvalidArgument("unknown command " + command);
    }
    return true;
}

int main() {
    Fleet fleet;
    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            if (!handleCommand(fleet, line)) {
                break;
            }
        } catch (const FleetError& e) {
            std::cout << "error: " << e.what() << "\n";
        }
    }
}