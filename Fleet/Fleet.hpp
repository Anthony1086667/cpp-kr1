#pragma once
#include "Errors.hpp"
#include "Inventory.hpp"
#include "Registry.hpp"
#include "ServiceCenter.hpp"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Fleet {
private:
    std::vector<std::shared_ptr<InventoryItem>> items_;
    int noviceThreshold_ = 6;
    ServiceCenter serviceCenter_;
    std::weak_ptr<InventoryItem> focus_;
    Registry<std::string, 100> rejected_;
    static bool lessById(const std::shared_ptr<InventoryItem>& a,
                         const std::shared_ptr<InventoryItem>& b) {
        return *a < *b;
    }

public:
    void addWithInspection(std::unique_ptr<InventoryItem> item) {
        try {
            serviceCenter_.inspect(*item);
        } catch (const InspectionRejected&) {
            rejected_.add(item->inventoryId() + " " + item->name());
            throw;
        }
        add(std::move(item));
    }

    void add(std::unique_ptr<InventoryItem> item) {
        if (find(item->inventoryId())) {
            throw InvalidArgument("duplicate id " + item->inventoryId());
        }
        items_.push_back(std::move(item));
    }

    void remove(const std::string& id) {
        for (std::size_t i = 0; i < items_.size(); ++i) {
            if (items_[i]->inventoryId() == id) {
                items_.erase(items_.begin() + i);
                return;
            }
        }
        throw InvalidArgument("no item " + id);
    }
    double totalEnergy() const {
        double total = 0.0;
        for (const auto& item : items_) {
            const EnergyConsumer* consumer = item->asEnergyConsumer();
            if (consumer != nullptr) {
                total += consumer->dailyEnergy();
            }
        }
        return total;
    }

    void printNovice(std::ostream& out) const {
        for (const auto& item : items_) {
            if (item->isNoviceFriendly(noviceThreshold_)) {
                out << *item << "\n";
            }
        }
    }

    void printList(std::ostream& out) const {
        std::vector<std::shared_ptr<InventoryItem>> sorted = items_;
        std::sort(sorted.begin(), sorted.end(), lessById);
        for (const auto& item : sorted) {
            out << *item << "\n";
        }
    }
    void printInfo(const std::string& id, std::ostream& out) const {
        std::shared_ptr<InventoryItem> item = find(id);
        if (!item) {
            throw InvalidArgument("no item " + id);
        }
        out << item->describe() << "\n";
    }

    void printRejected(std::ostream& out) const {
        out << rejected_;
    }

    void setFocus(const std::string& id) {
        std::shared_ptr<InventoryItem> item = find(id);
        if (!item) {
            throw InvalidArgument("no item " + id);
        }
        focus_ = item;
    }

    void printFocus(std::ostream& out) const {
        std::shared_ptr<InventoryItem> item = focus_.lock();
        if (item) {
            out << *item << "\n";
        } else {
            out << "expired\n";
        }
    }

    std::shared_ptr<InventoryItem> find(const std::string& id) const {
        for (const auto& item : items_) {
            if (item->inventoryId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    std::size_t size() const {
        return items_.size();
    }
};