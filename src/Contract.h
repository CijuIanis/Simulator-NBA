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

    double getSalaryPerYear() const { return salaryPerYear; }
    int getDurationYears() const { return durationYears; }
    const std::string& getType() const { return type; }
    double getTotalValue() const { return salaryPerYear * durationYears; }
    bool isMaxContract() const { return type == "MAX"; }
    double getRemainingValue(int yearsPlayed) const;

    friend std::ostream& operator<<(std::ostream& os, const Contract& contract);
};