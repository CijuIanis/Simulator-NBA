#pragma once
#include "Player.h"

class TwoWayPlayer : public Player {
private:
    double defensiveRating;

protected:
    void print(std::ostream& os) const override;

public:
    TwoWayPlayer(const std::string& name, int age, const std::string& position,
                 double ppg, double apg, double rpg, const Contract& contract,
                 double defensiveRating);

    TwoWayPlayer* clone() const override;
    double calculateRoleScore() const override;
    std::string getRoleDescription() const override;
    double getDefensiveRating() const;
};