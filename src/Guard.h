#pragma once
#include "Player.h"

class Guard : public Player {
private:
    double threePointPercentage;

protected:
    void print(std::ostream& os) const override;

public:
    Guard(const std::string& name, int age, const std::string& position,
          double ppg, double apg, double rpg, const Contract& contract,
          double threePointPercentage);

    Guard* clone() const override;
    double calculateRoleScore() const override;
    std::string getRoleDescription() const override;
};