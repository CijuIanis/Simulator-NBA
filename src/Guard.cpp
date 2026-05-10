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
    // Guard-ii sunt evaluati dupa pase si puncte de la distanta
    return assistsPerGame * 2.0 + pointsPerGame * 1.0 + threePointPercentage * 10.0;
}

double Guard::getThreePointPercentage() const {
    return threePointPercentage;
}

void Guard::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << threePointPercentage * 100.0;
    os << "\n  [Guard] 3PT%: " << oss.str() << "% | Role Score: " << calculateRoleScore();
}