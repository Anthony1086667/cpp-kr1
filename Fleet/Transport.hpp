#pragma once
#include "Errors.hpp"
#include "Inventory.hpp"
#include <string>

class Transport : public InventoryItem {
private:
    std::string id_;
    std::string name_;
    int simplicity_;

public:
    Transport(std::string id, std::string name, int simplicity)
        : id_(id), name_(name), simplicity_(simplicity) {
        if (id_.empty()) {
            throw InvalidArgument("empty id");
        }
        if (simplicity_ < 1 || simplicity_ > 10) {
            throw InvalidArgument("simplicity must be 1..10");
        }
    }
    bool canBeLocked() const {
        return true;
    }

    std::string inventoryId() const {
        return id_;
    }

    std::string name() const {
        return name_;
    }

    bool isNoviceFriendly(int minSimplicity) const {
        return simplicity_ >= minSimplicity;
    }
};

class Bike : public Transport {
public:
    Bike(std::string id, std::string name, int simplicity)
        : Transport(id, name, simplicity) {}

    std::string describe() const {
        return "bike " + inventoryId() + " " + name();
    }
};

class Scooter : public Transport, public EnergyConsumer {
private:
    double energy_;

public:
    Scooter(std::string id, std::string name, int simplicity, double energy)
        : Transport(id, name, simplicity), energy_(energy) {
        if (energy_ < 0) {
            throw InvalidArgument("energy must be >= 0");
        }
    }

    std::string describe() const {
        return "scooter " + inventoryId() + " " + name();
    }

    double dailyEnergy() const {
        return energy_;
    }

    const EnergyConsumer* asEnergyConsumer() const {
        return this;
    }
};

class Ebike : public Transport, public EnergyConsumer {
private:
    double energy_;

public:
    Ebike(std::string id, std::string name, int simplicity, double energy)
        : Transport(id, name, simplicity), energy_(energy) {
        if (energy_ < 0) {
            throw InvalidArgument("energy must be >= 0");
        }
    }

    std::string describe() const {
        return "ebike " + inventoryId() + " " + name();
    }

    double dailyEnergy() const {
        return energy_;
    }

    const EnergyConsumer* asEnergyConsumer() const {
        return this;
    }
};
