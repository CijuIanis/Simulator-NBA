#include "TwoWayPlayer.h"
#include <sstream>
#include <iomanip>

TwoWayPlayer::TwoWayPlayer(const std::string& name, int age, const std::string& position,
                           double ppg, double apg, double rpg, const Contract& contract,
                           double defensiveRating)
    : Player(name, age, position, ppg, apg, rpg, contract),
      defensiveRating(defensiveRating) {}

TwoWayPlayer* TwoWayPlayer::clone() const {
    return new TwoWayPlayer(*this);
}

double TwoWayPlayer::calculateRoleScore() const {
    // TwoWayPlayer: echilibru intre ofensiva si defensiva
    return pointsPerGame * 1.0 + reboundsPerGame * 1.0 +
           assistsPerGame * 1.0 + defensiveRating * 2.0;
}
// cppcheck-suppress unusedFunction
double TwoWayPlayer::getDefensiveRating() const {
    return defensiveRating;
}

void TwoWayPlayer::print(std::ostream& os) const {
    Player::print(os);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << defensiveRating;
    os << "\n  [TwoWay] DEF Rating: " << oss.str()
       << " | Role Score: " << calculateRoleScore();
}