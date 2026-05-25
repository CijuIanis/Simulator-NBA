#pragma once
#include "Player.h"

class Forward : public Player {
private:
    double fieldGoalPercentage;

protected:
    void print(std::ostream& os) const override;

public:
    Forward(const std::string& name, int age, const std::string& position,
            double ppg, double apg, double rpg, const Contract& contract,
            double fieldGoalPercentage);

    Forward* clone() const override;
    double calculateRoleScore() const override;
    std::string getRoleDescription() const override;
    double getFieldGoalPercentage() const;
};