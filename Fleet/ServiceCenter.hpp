#pragma once

#include "Inventory.hpp"
#include "Errors.hpp"
#include <cctype>
#include <string>

class ServiceCenter {
public:
    void inspect(const InventoryItem& item) const {
        std::string name = item.name();
        for (char& c : name) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        if (name.find("broken") != std::string::npos) {
            throw InspectionRejected("inspection rejected");
        }
    }
};