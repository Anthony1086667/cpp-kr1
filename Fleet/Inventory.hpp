#pragma once
#include <ostream>
#include <string>

class EnergyConsumer {
public:
    virtual ~EnergyConsumer() = default;
    virtual double dailyEnergy() const = 0;
};

class InventoryItem {
public:
    virtual ~InventoryItem() = default;
    virtual std::string inventoryId() const = 0;
    virtual std::string name() const = 0;
    virtual std::string describe() const = 0;
    virtual bool canBeLocked() const { return false; }
    virtual bool isNoviceFriendly(int) const { return false; }
    virtual const EnergyConsumer* asEnergyConsumer() const { return nullptr; }
};

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
    out << item.inventoryId() << " " << item.name();
    return out;
}

bool operator<(const InventoryItem& a, const InventoryItem& b) {
    return a.inventoryId() < b.inventoryId();
}