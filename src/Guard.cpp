#include "Guard.h"
#include <sstream>
#include <iomanip>

Guard::Guard(const std::string& name, int age, const std::string& position,
             double ppg, double apg, double rpg, const Contract& contract,
             double threePointPercentage)
    : Player(name, age, position, ppg, apg, rpg, contract),
      threePointPercentage(threePointPercentage) {}

Guard* Guard::clone() const {
    return new Guard(*this);
}

double Guard::calculateRoleScore() const {
    return assistsPerGame * 2.0 + pointsPerGame * 1.0 + threePointPercentage * 10.0;
}

std::string Guard::getRoleDescription() const {
    // clasificam Guard in functie de stil de joc
    if (assistsPerGame >= 7.0 && pointsPerGame < 20.0)
        return "Pure Point Guard - playmaker, paseaza mult mingea";
    if (pointsPerGame >= 22.0 && assistsPerGame < 5.0)
        return "Scoring Guard - marcheaza puncte multe";
    if (threePointPercentage >= 0.38 && pointsPerGame >= 15.0)
        return "Sharpshooter - specialist 3 puncte";
    if (assistsPerGame >= 5.0 && pointsPerGame >= 18.0)
        return "Combo Guard - mixt intre scoring si playmaking";
    return "Role Player Guard - rol de sprijin";
}

void Guard::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << threePointPercentage * 100.0;
    os << "\n  [Guard] 3PT%: " << oss.str() << "% | Role Score: " << calculateRoleScore()
       << "\n  Tip: " << getRoleDescription();
}