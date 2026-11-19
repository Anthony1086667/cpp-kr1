#pragma once
#include <stdexcept>
#include <string>

class FleetError : public std::runtime_error {
public:
    FleetError(const std::string& message)
        : std::runtime_error(message) {}
};

class InspectionRejected : public FleetError {
public:
    InspectionRejected(const std::string& message)
        : FleetError(message) {}
};

class InvalidArgument : public FleetError {
public:
    InvalidArgument(const std::string& message)
        : FleetError(message) {}
};