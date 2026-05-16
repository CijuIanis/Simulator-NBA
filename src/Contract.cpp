#include "Contract.h"

Contract::Contract(double salaryPerYear, int durationYears, const std::string& type)
    : salaryPerYear(salaryPerYear), durationYears(durationYears), type(type) {}

Contract::Contract(const Contract& other)
    : salaryPerYear(other.salaryPerYear), durationYears(other.durationYears), type(other.type) {}

Contract& Contract::operator=(const Contract& other) {
    if (this != &other) {
        salaryPerYear = other.salaryPerYear;
        durationYears = other.durationYears;
        type = other.type;
    }
    return *this;
}

Contract::~Contract() {}

double Contract::getRemainingValue(int yearsPlayed) const {
    int yearsLeft = durationYears - yearsPlayed;
    if (yearsLeft <= 0) return 0.0;
    return salaryPerYear * yearsLeft;
}

std::ostream& operator<<(std::ostream& os, const Contract& contract) {
    os << "Contract [" << contract.type << "] | "
       << "$" << contract.salaryPerYear << "M/an | "
       << contract.durationYears << " ani | "
       << "Total: $" << contract.getTotalValue() << "M";
    return os;
}