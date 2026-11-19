#pragma once
#include "Errors.hpp"
#include <ostream>
#include <string>

template<typename T>
void printEntry(std::ostream& out, const T& value) {
    out << value << " ";
}

template<>
void printEntry<std::string>(std::ostream& out, const std::string& value) {
    out << value << "\n";
}

template<typename T, int Cap>
class Registry {
private:
    T items_[Cap];
    int size_ = 0;

public:
    void add(const T& value) {
        if (size_ >= Cap) {
            throw InvalidArgument("registry is full");
        }
        items_[size_] = value;
        ++size_;
    }

    int size() const {
        return size_;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size_) {
            throw InvalidArgument("registry index out of range");
        }
        return items_[index];
    }
};

template<typename T, int Cap>
std::ostream& operator<<(std::ostream& out, const Registry<T, Cap>& registry) {
    for (int i = 0; i < registry.size(); ++i) {
        printEntry(out, registry[i]);
    }
    return out;
}