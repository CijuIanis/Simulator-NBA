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
    return pointsPerGame * 1.0 + reboundsPerGame * 1.0 +
           assistsPerGame * 1.0 + defensiveRating * 2.0;
}

std::string TwoWayPlayer::getRoleDescription() const {
    // clasificam TwoWayPlayer in functie de balansul ofensiv/defensiv
    if (defensiveRating >= 9.0 && pointsPerGame >= 20.0)
        return "Elite Two-Way Star - top atat in atac cat si defensiv";
    if (defensiveRating >= 9.0)
        return "Elite Defender - specialist defensiv";
    if (defensiveRating >= 7.0 && pointsPerGame >= 15.0)
        return "Two-Way Wing - jucator complet pe ambele parti";
    if (pointsPerGame >= 18.0 && defensiveRating >= 5.0)
        return "Two-Way Scorer - scoring cu defensiva solida";
    return "Two-Way Role Player - contribuie pe ambele parti";
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
       << " | Role Score: " << calculateRoleScore()
       << "\n  Tip: " << getRoleDescription();
}