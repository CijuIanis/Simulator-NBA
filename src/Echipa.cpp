#include "Echipa.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <tabulate/table.hpp>

double Echipa::calculeazaSalariiTotale() const {
    double total = 0.0;
    for (const auto& player : roster)
        total += player.getContract().getSalaryPerYear();
    return total;
}

Echipa::Echipa(const std::string& nume, const std::string& oras,
               const std::string& conferinta, double salaryCap)
    : nume(nume), oras(oras), conferinta(conferinta), salaryCap(salaryCap) {}

const std::string& Echipa::getNume() const { return nume; }
const std::string& Echipa::getOras() const { return oras; }
const std::string& Echipa::getConferinta() const { return conferinta; }
double Echipa::getSalaryCap() const { return salaryCap; }
const std::vector<Player>& Echipa::getRoster() const { return roster; }

void Echipa::adaugaJucator(const Player& player) {
    roster.push_back(player);
}

double Echipa::getScorImpact() const {
    double total = 0.0;
    for (const auto& player : roster)
        total += player.getImpactScore();
    return total;
}

bool Echipa::esteSubSalaryCap() const {
    return calculeazaSalariiTotale() <= salaryCap;
}

const Player& Echipa::getCelMaiBunJucator() const {
    if (roster.empty())
        throw std::runtime_error("Echipa nu are jucatori!");

    const Player* best = &roster[0];
    for (const auto& player : roster)
        if (player.getImpactScore() > best->getImpactScore())
            best = &player;
    return *best;
}

std::ostream& operator<<(std::ostream& os, const Echipa& echipa) {
    os << "=== " << echipa.oras << " " << echipa.nume
       << " [" << echipa.conferinta << "] ===\n"
       << "Salary Cap: $" << echipa.salaryCap << "M | "
       << "Salarii totale: $" << std::fixed << std::setprecision(2)
       << echipa.calculeazaSalariiTotale() << "M | "
       << (echipa.esteSubSalaryCap() ? "Sub cap" : "Peste cap") << "\n";

    tabulate::Table tabel;
    tabel.add_row({"Pozitie", "Nume", "Varsta", "PPG", "APG", "RPG", "Tip", "Salariu", "Statut"});

    for (const auto& player : echipa.roster) {
        std::ostringstream ppg, apg, rpg, sal;
        ppg << std::fixed << std::setprecision(1) << player.getPointsPerGame();
        apg << std::fixed << std::setprecision(1) << player.getAssistsPerGame();
        rpg << std::fixed << std::setprecision(1) << player.getReboundsPerGame();
        sal << "$" << std::fixed << std::setprecision(2) << player.getContract().getSalaryPerYear() << "M";

        tabel.add_row({
            player.getPosition(),
            player.getName(),
            std::to_string(player.getAge()),
            ppg.str(),
            apg.str(),
            rpg.str(),
            player.getContract().getType(),
            sal.str(),
            player.isAllStar() ? "All-Star" : "Role Player"
        });
    }

    // header galben si bold
    tabel[0].format()
        .font_style({tabulate::FontStyle::bold})
        .font_align(tabulate::FontAlign::center)
        .font_color(tabulate::Color::yellow);

    // All-Star verde, Role Player albastru
    for (auto i = 1u; i <= echipa.roster.size(); i++) {
        if (echipa.roster[i - 1].isAllStar()) {
            tabel[i].format().font_color(tabulate::Color::green);
        } else {
            tabel[i].format().font_color(tabulate::Color::blue);
        }
    }

    os << tabel << "\n";
    return os;
}