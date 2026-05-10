#include "Forward.h"
#include <sstream>
#include <iomanip>

Forward::Forward(const std::string& name, int age, const std::string& position,
                 double ppg, double apg, double rpg, const Contract& contract,
                 double fieldGoalPercentage)
    : Player(name, age, position, ppg, apg, rpg, contract),
      fieldGoalPercentage(fieldGoalPercentage) {}

Forward* Forward::clone() const {
    return new Forward(*this);
}

double Forward::calculateRoleScore() const {
    // Forward-ii sunt evaluati dupa versatilitate: puncte + recuperari
    return pointsPerGame * 1.5 + reboundsPerGame * 1.5 + fieldGoalPercentage * 10.0;
}

double Forward::getFieldGoalPercentage() const {
    return fieldGoalPercentage;
}

void Forward::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << fieldGoalPercentage * 100.0;
    os << "\n  [Forward] FG%: " << oss.str() << "% | Role Score: " << calculateRoleScore();
}