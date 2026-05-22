#include "Echipa.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <tabulate/table.hpp>

double Echipa::calculeazaSalariiTotale() const {
    double total = 0.0;
    for (const auto& player : roster)
        total += player->getContract().getSalaryPerYear();
    return total;
}

Echipa::Echipa(const std::string& nume, const std::string& oras,
               const std::string& conferinta, double salaryCap)
    : nume(nume), oras(oras), conferinta(conferinta), salaryCap(salaryCap) {}

Echipa::Echipa(const Echipa& other)
    : nume(other.nume), oras(other.oras),
      conferinta(other.conferinta), salaryCap(other.salaryCap) {
    for (const auto& player : other.roster)
        roster.push_back(std::unique_ptr<Player>(player->clone()));
}

void swap(Echipa& a, Echipa& b) noexcept {
    std::swap(a.nume, b.nume);
    std::swap(a.oras, b.oras);
    std::swap(a.conferinta, b.conferinta);
    std::swap(a.salaryCap, b.salaryCap);
    std::swap(a.roster, b.roster);
}

Echipa& Echipa::operator=(Echipa other) {
    swap(*this, other);
    return *this;
}

const std::string& Echipa::getNume() const { return nume; }
const std::string& Echipa::getOras() const { return oras; }
const std::string& Echipa::getConferinta() const { return conferinta; }
double Echipa::getSalaryCap() const { return salaryCap; }
const std::vector<std::unique_ptr<Player>>& Echipa::getRoster() const { return roster; }

void Echipa::adaugaJucator(const Player& player) {
    roster.push_back(std::unique_ptr<Player>(player.clone()));
}

double Echipa::getScorImpact() const {
    double total = 0.0;
    for (const auto& player : roster)
        total += player->getImpactScore();
    return total;
}

bool Echipa::esteSubSalaryCap() const {
    return calculeazaSalariiTotale() <= salaryCap;
}

int Echipa::getNrJucatoriMaxContract() const {
    int count = 0;
    for (const auto& player : roster)
        if (player->getContract().isMaxContract())
            count++;
    return count;
}

std::vector<const Player*> Echipa::getJucatoriAllStar() const {
    std::vector<const Player*> allStars;
    for (const auto& player : roster)
        if (player->isAllStar())
            allStars.push_back(player.get());
    return allStars;
}

// cppcheck-suppress unusedFunction
int Echipa::getNrAllStars() const {
    int count = 0;
    for (const auto& player : roster)
        if (player->isAllStar())
            count++;
    return count;
}

// cppcheck-suppress unusedFunction
bool Echipa::isContender() const {
    return getNrAllStars() >= 2;
}

// cppcheck-suppress unusedFunction
int Echipa::getNrJucatoriExpiring() const {
    int count = 0;
    for (const auto& player : roster)
        if (player->getContract().isExpiring())
            count++;
    return count;
}

double Echipa::getValoareRoster() const {
    double total = 0.0;
    for (const auto& player : roster)
        total += player->getContract().getTotalValue();
    return total;
}

const Player& Echipa::getCelMaiBunJucator() const {
    if (roster.empty())
        throw std::runtime_error("Echipa nu are jucatori!");

    const Player* best = roster[0].get();
    for (const auto& player : roster)
        if (player->getImpactScore() > best->getImpactScore())
            best = player.get();
    return *best;
}

std::ostream& operator<<(std::ostream& os, const Echipa& echipa) {
    os << "=== " << echipa.oras << " " << echipa.nume
       << " [" << echipa.conferinta << "] ===\n"
       << "Salary Cap: $" << echipa.salaryCap << "M | "
       << "Salarii totale: $" << std::fixed << std::setprecision(2)
       << echipa.calculeazaSalariiTotale() << "M | "
       << (echipa.esteSubSalaryCap() ? "Sub cap" : "Peste cap") << " | "
       << "Contracte MAX: " << echipa.getNrJucatoriMaxContract() << "\n";

    tabulate::Table tabel;
    tabel.add_row({"Pozitie", "Nume", "Varsta", "PPG", "APG", "RPG", "Tip", "Salariu", "Statut"});

    for (const auto& player : echipa.roster) {
        std::ostringstream ppg, apg, rpg, sal;
        ppg << std::fixed << std::setprecision(1) << player->getPointsPerGame();
        apg << std::fixed << std::setprecision(1) << player->getAssistsPerGame();
        rpg << std::fixed << std::setprecision(1) << player->getReboundsPerGame();
        sal << "$" << std::fixed << std::setprecision(2) << player->getContract().getSalaryPerYear() << "M";

        tabel.add_row({
            player->getPosition(),
            player->getName(),
            std::to_string(player->getAge()),
            ppg.str(),
            apg.str(),
            rpg.str(),
            player->getContract().getType(),
            sal.str(),
            player->isAllStar() ? "All-Star" : "Role Player"
        });
    }

    tabel[0].format()
        .font_style({tabulate::FontStyle::bold})
        .font_align(tabulate::FontAlign::center)
        .font_color(tabulate::Color::yellow);

    for (auto i = 1u; i <= echipa.roster.size(); i++) {
        if (echipa.roster[i - 1]->isAllStar())
            tabel[i].format().font_color(tabulate::Color::green);
        else
            tabel[i].format().font_color(tabulate::Color::blue);
    }

    os << tabel << "\n";
    return os;
}