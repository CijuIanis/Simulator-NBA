#pragma once
#include <string>
#include <ostream>

class Contract {
private:
    double salaryPerYear;
    int durationYears;
    std::string type;

public:
    Contract(double salaryPerYear, int durationYears, const std::string& type);
    Contract(const Contract& other);
    Contract& operator=(const Contract& other);
    ~Contract();

    // cppcheck-suppress unusedFunction
    double getSalaryPerYear() const { return salaryPerYear; }
    // cppcheck-suppress unusedFunction
    int getDurationYears() const { return durationYears; }
    // cppcheck-suppress unusedFunction
    const std::string& getType() const { return type; }
    double getTotalValue() const { return salaryPerYear * durationYears; }
    // cppcheck-suppress unusedFunction
    bool isMaxContract() const { return type == "MAX"; }
    // cppcheck-suppress unusedFunction
    bool isExpiring() const { return durationYears == 1; }
    double getRemainingValue(int yearsPlayed) const;

    friend std::ostream& operator<<(std::ostream& os, const Contract& contract);
};