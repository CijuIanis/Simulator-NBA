#include "Center.h"
#include <sstream>
#include <iomanip>

Center::Center(const std::string& name, int age, const std::string& position,
               double ppg, double apg, double rpg, const Contract& contract,
               double blocksPerGame)
    : Player(name, age, position, ppg, apg, rpg, contract),
      blocksPerGame(blocksPerGame) {}

Center* Center::clone() const {
    return new Center(*this);
}

double Center::calculateRoleScore() const {
    return reboundsPerGame * 2.0 + blocksPerGame * 3.0 + pointsPerGame * 0.5;
}

std::string Center::getRoleDescription() const {
    // clasificam Center in functie de stil de joc
    if (blocksPerGame >= 2.5 && reboundsPerGame >= 10.0)
        return "Defensive Anchor - protector elite al cosului";
    if (pointsPerGame >= 22.0 && reboundsPerGame >= 10.0)
        return "Dominant Big - double double garantat";
    if (assistsPerGame >= 4.0 && pointsPerGame >= 15.0)
        return "Playmaking Big - flashy pentru un pivot";
    if (pointsPerGame >= 15.0 && blocksPerGame < 1.5)
        return "Stretch 5 - joaca si sub panou, dar mai mult din afara";
    if (reboundsPerGame >= 8.0 && blocksPerGame >= 1.0)
        return "Traditional Big - dominant ca pivot";
    return "Role Player Center - rol de sprijin";
}

// cppcheck-suppress unusedFunction
double Center::getBlocksPerGame() const {
    return blocksPerGame;
}

void Center::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << blocksPerGame;
    os << "\n  [Center] BLK: " << oss.str() << " | Role Score: " << calculateRoleScore()
       << "\n  Tip: " << getRoleDescription();
}