#pragma once
#include "Player.h"

class Center : public Player {
private:
    double blocksPerGame;

protected:
    void print(std::ostream& os) const override;

public:
    Center(const std::string& name, int age, const std::string& position,
           double ppg, double apg, double rpg, const Contract& contract,
           double blocksPerGame);

    Center* clone() const override;
    double calculateRoleScore() const override;
    std::string getRoleDescription() const override;
    double getBlocksPerGame() const;
};