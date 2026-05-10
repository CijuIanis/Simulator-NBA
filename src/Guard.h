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

    // Scorul specific unui Guard: bazat pe pase si puncte
    double calculateRoleScore() const override;

    double getThreePointPercentage() const;
};