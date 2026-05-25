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
    return pointsPerGame * 1.5 + reboundsPerGame * 1.5 + fieldGoalPercentage * 10.0;
}

std::string Forward::getRoleDescription() const {
    // clasificam Forward in functie de stil de joc
    if (pointsPerGame >= 25.0 && fieldGoalPercentage >= 0.50)
        return "Elite Scorer - marcheaza eficient si mult";
    if (reboundsPerGame >= 8.0 && pointsPerGame >= 15.0)
        return "Power Forward Clasic - dominant sub panou";
    if (fieldGoalPercentage >= 0.50 && pointsPerGame < 15.0)
        return "Stretch 4 - shooter de la distanta";
    if (pointsPerGame >= 18.0 && assistsPerGame >= 3.0)
        return "Wing Scorer - versatilitate ofensiva";
    if (reboundsPerGame >= 6.0 && fieldGoalPercentage >= 0.45)
        return "3-and-D - shooter si defensive";
    return "Role Player Forward - rol de sprijin";
}

// cppcheck-suppress unusedFunction
double Forward::getFieldGoalPercentage() const {
    return fieldGoalPercentage;
}

void Forward::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << fieldGoalPercentage * 100.0;
    os << "\n  [Forward] FG%: " << oss.str() << "% | Role Score: " << calculateRoleScore()
       << "\n  Tip: " << getRoleDescription();
}