#pragma once
#include <string>
#include <ostream>
#include "Contract.h"

class Player {
protected:
    std::string name;
    int age;
    std::string position;
    double pointsPerGame;
    double assistsPerGame;
    double reboundsPerGame;
    Contract contract;

    double calculateEfficiencyRating() const;
    virtual void print(std::ostream& os) const;

private:
    static int totalJucatoriCreati;

public:
    Player(const std::string& name, int age, const std::string& position,
           double ppg, double apg, double rpg, const Contract& contract);

    virtual ~Player() = default;
    virtual Player* clone() const = 0;
    virtual double calculateRoleScore() const = 0;

    const std::string& getName() const;
    int getAge() const;
    const std::string& getPosition() const;
    double getPointsPerGame() const;
    double getAssistsPerGame() const;
    double getReboundsPerGame() const;
    const Contract& getContract() const;

    double getImpactScore() const;
    bool isAllStar() const;
    std::string getStatLine() const;

    bool isBetterThan(const Player& other) const {
        return getImpactScore() > other.getImpactScore();
    }

    static int getTotalJucatori() { return totalJucatoriCreati; }

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};