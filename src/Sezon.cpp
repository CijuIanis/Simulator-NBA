#include "Sezon.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <tabulate/table.hpp>

Sezon::Sezon(const std::string& an, const std::string& campioana)
    : an(an), campioana(campioana), nrEchipe(0) {}

const std::string& Sezon::getAn() const { return an; }
const std::string& Sezon::getCampioana() const { return campioana; }
int Sezon::getNrEchipe() const { return nrEchipe; }
const std::vector<Echipa>& Sezon::getEchipe() const { return echipe; }

void Sezon::adaugaEchipa(const Echipa& echipa) {
    echipe.push_back(echipa);
    nrEchipe++;
}

const Echipa& Sezon::getEchipaFavorita() const {
    if (echipe.empty())
        throw std::runtime_error("Sezonul nu are echipe!");

    const Echipa* favorita = &echipe[0];
    for (const auto& echipa : echipe)
        if (echipa.getScorImpact() > favorita->getScorImpact())
            favorita = &echipa;
    return *favorita;
}

const Player& Sezon::getCelMaiBunJucatorDinSezon() const {
    if (echipe.empty())
        throw std::runtime_error("Sezonul nu are echipe!");

    const Player* best = &echipe[0].getCelMaiBunJucator();
    for (const auto& echipa : echipe) {
        const Player& candidat = echipa.getCelMaiBunJucator();
        if (candidat.getImpactScore() > best->getImpactScore())
            best = &candidat;
    }
    return *best;
}

std::map<std::string, std::vector<const Echipa*>> Sezon::getEchipeDupaConferinta() const {
    std::map<std::string, std::vector<const Echipa*>> rezultat;
    for (const auto& echipa : echipe)
        rezultat[echipa.getConferinta()].push_back(&echipa);
    return rezultat;
}

void Sezon::afiseazaClassament() const {
    std::vector<const Echipa*> sorted;
    for (const auto& echipa : echipe)
        sorted.push_back(&echipa);

    std::sort(sorted.begin(), sorted.end(), [](const Echipa* a, const Echipa* b) {
        return a->getScorImpact() > b->getScorImpact();
    });

    tabulate::Table tabel;
    tabel.add_row({"#", "Echipa", "Oras", "Conferinta", "Impact"});

    for (auto i = 0u; i < sorted.size(); i++) {
        std::ostringstream impact;
        impact << std::fixed << std::setprecision(1) << sorted[i]->getScorImpact();
        tabel.add_row({
            std::to_string(i + 1),
            sorted[i]->getNume(),
            sorted[i]->getOras(),
            sorted[i]->getConferinta(),
            impact.str()
        });

        if (i == 0)
            tabel[i + 1].format().font_color(tabulate::Color::yellow);
        else if (i == 1)
            tabel[i + 1].format().font_color(tabulate::Color::cyan);
        else if (i == 2)
            tabel[i + 1].format().font_color(tabulate::Color::magenta);
    }

    tabel[0].format()
        .font_style({tabulate::FontStyle::bold})
        .font_align(tabulate::FontAlign::center)
        .font_color(tabulate::Color::blue);

    std::cout << "\nClasament Sezon " << an << ":\n";
    std::cout << tabel << "\n";
    std::cout << "Campioana reala: " << campioana << "\n";
}

std::ostream& operator<<(std::ostream& os, const Sezon& sezon) {
    os << "Sezon NBA " << sezon.an << "\n"
       << "Echipe: " << sezon.nrEchipe << "\n"
       << "Campioana: " << sezon.campioana << "\n";
    for (const auto& echipa : sezon.echipe)
        os << echipa << "\n";
    return os;
}

const Player* Sezon::gasesteJucator(const std::string& nume) const {
    for (const auto& echipa : echipe)
        for (const auto& player : echipa.getRoster())
            if (player->getName() == nume)
                return player.get();
    return nullptr;
}