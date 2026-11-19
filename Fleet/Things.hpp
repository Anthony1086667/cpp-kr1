#pragma once
#include "Errors.hpp"
#include "Inventory.hpp"
#include <memory>
#include <string>

class Thing : public InventoryItem {
private:
    std::string id_;
    std::string name_;

public:
    Thing(std::string id, std::string name) : id_(id), name_(name) {
        if (id_.empty()) {
            throw InvalidArgument("empty id");
        }
    }

    std::string inventoryId() const {
        return id_;
    }

    std::string name() const {
        return name_;
    }
};

class Helmet : public Thing {
public:
    Helmet(std::string id, std::string name) : Thing(id, name) {}

    std::string describe() const {
        return "helmet " + inventoryId() + " " + name();
    }
};

class Dock : public Thing, public EnergyConsumer {
private:
    double energy_;

public:
    Dock(std::string id, std::string name, double energy)
        : Thing(id, name), energy_(energy) {
        if (energy_ < 0) {
            throw InvalidArgument("energy must be >= 0");
        }
    }

    std::string describe() const {
        return "dock " + inventoryId() + " " + name();
    }

    double dailyEnergy() const {
        return energy_;
    }

    const EnergyConsumer* asEnergyConsumer() const {
        return this;
    }
};

class Robot : public Thing, public EnergyConsumer {
private:
    double energy_;
    int capacity_;

public:
    Robot(std::string id, std::string name, double energy, int capacity)
        : Thing(id, name), energy_(energy), capacity_(capacity) {
        if (energy_ < 0) {
            throw InvalidArgument("energy must be >= 0");
        }
        if (capacity_ <= 0) {
            throw InvalidArgument("capacity must be > 0");
        }
    }

    std::string describe() const {
        return "robot " + inventoryId() + " " + name() +
               " capacity " + std::to_string(capacity_) + " kg";
    }

    double dailyEnergy() const {
        return energy_;
    }

    const EnergyConsumer* asEnergyConsumer() const {
        return this;
    }
};

class Lock : public Thing, public EnergyConsumer {
private:
    double energy_;
    std::weak_ptr<InventoryItem> target_;

public:
    Lock(std::string id, std::string name, double energy,
         const std::shared_ptr<InventoryItem>& target)
        : Thing(id, name), energy_(energy), target_(target) {
        if (energy_ < 0) {
            throw InvalidArgument("energy must be >= 0");
        }
        if (!target) {
            throw InvalidArgument("lock target not found");
        }
    }

    std::string describe() const {
        std::string result = "lock " + inventoryId() + " " + name();
        std::shared_ptr<InventoryItem> target = target_.lock();
        if (target) {
            result += " on " + target->inventoryId();
        } else {
            result += " (target removed)";
        }
        return result;
    }

    double dailyEnergy() const {
        return energy_;
    }

    const EnergyConsumer* asEnergyConsumer() const {
        return this;
    }
};