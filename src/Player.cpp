#include "Player.h"
#include <sstream>
#include <iomanip>

int Player::totalJucatoriCreati = 0;

double Player::calculateEfficiencyRating() const {
    return pointsPerGame + assistsPerGame * 0.75 + reboundsPerGame * 0.5;
}

Player::Player(const std::string& name, int age, const std::string& position,
               double ppg, double apg, double rpg, const Contract& contract)
    : name(name), age(age), position(position),
      pointsPerGame(ppg), assistsPerGame(apg), reboundsPerGame(rpg),
      contract(contract) {
    totalJucatoriCreati++;
}

const std::string& Player::getName() const { return name; }
int Player::getAge() const { return age; }
const std::string& Player::getPosition() const { return position; }
double Player::getPointsPerGame() const { return pointsPerGame; }
double Player::getAssistsPerGame() const { return assistsPerGame; }
double Player::getReboundsPerGame() const { return reboundsPerGame; }
const Contract& Player::getContract() const { return contract; }

double Player::getImpactScore() const {
    return calculateEfficiencyRating() * (isAllStar() ? 1.5 : 1.0);
}

bool Player::isAllStar() const {
    return pointsPerGame >= 18.0 ||
           (pointsPerGame >= 14.0 && assistsPerGame >= 7.0) ||
           (pointsPerGame >= 14.0 && reboundsPerGame >= 8.0);
}

std::string Player::getStatLine() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1)
        << pointsPerGame << "pts | "
        << assistsPerGame << "ast | "
        << reboundsPerGame << "reb";
    return oss.str();
}

void Player::print(std::ostream& os) const {
    os << "[" << position << "] "
       << name
       << " (age: " << age << ") | "
       << getStatLine() << " | "
       << (isAllStar() ? "All-Star" : "Role Player") << "\n"
       << "  " << contract;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    player.print(os);
    return os;
}