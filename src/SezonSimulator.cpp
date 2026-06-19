#include "SezonSimulator.h"
#include "Stats.h"
#include <algorithm>
#include <utility>

// cppcheck-suppress unusedFunction
void SezonSimulator::adaugaObserver(Observer* obs) {
    observatori.push_back(obs);
}

void SezonSimulator::notificaInceput(const std::string& an) {
    for (Observer* obs : observatori)
        obs->onInceputPlayoff(an);
}

void SezonSimulator::notificaMeci(const Echipa& castigatoare, const Echipa& invinsa,
                                  const std::string& runda) {
    for (Observer* obs : observatori)
        obs->onMeci(castigatoare, invinsa, runda);
}

void SezonSimulator::notificaCampioana(const Echipa& campioana) {
    for (Observer* obs : observatori)
        obs->onCampioanaNBA(campioana);
}

const Echipa* SezonSimulator::simuleazaBracket(std::vector<const Echipa*> echipe,
                                               const std::string& numeConferinta) {
    std::sort(echipe.begin(), echipe.end(), [](const Echipa* a, const Echipa* b) {
        return a->getScorImpact() > b->getScorImpact();
    });
    if (echipe.size() > 8)
        echipe.resize(8);

    // Runda 1: 1vs8, 2vs7, 3vs6, 4vs5
    std::vector<const Echipa*> semifinalisti;
    std::vector<std::pair<int, int>> perechi = {{0, 7}, {1, 6}, {2, 5}, {3, 4}};
    for (const auto& [i, j] : perechi) {
        if (i < static_cast<int>(echipe.size()) && j < static_cast<int>(echipe.size())) {
            const Echipa* castig = Stats::simulateMeci(*echipe[i], *echipe[j]);
            const Echipa* invins = (castig == echipe[i]) ? echipe[j] : echipe[i];
            notificaMeci(*castig, *invins, numeConferinta + " R1");
            semifinalisti.push_back(castig);
        } else if (i < static_cast<int>(echipe.size())) {
            semifinalisti.push_back(echipe[i]);
        }
    }

    // Runda 2: semifinale conferinta
    std::vector<const Echipa*> finalisti;
    for (int i = 0; i + 1 < static_cast<int>(semifinalisti.size()); i += 2) {
        const Echipa* castig = Stats::simulateMeci(*semifinalisti[i], *semifinalisti[i + 1]);
        const Echipa* invins = (castig == semifinalisti[i]) ? semifinalisti[i + 1] : semifinalisti[i];
        notificaMeci(*castig, *invins, numeConferinta + " Semifinale");
        finalisti.push_back(castig);
    }

    // Finala conferinta
    if (finalisti.size() >= 2) {
        const Echipa* campion = Stats::simulateMeci(*finalisti[0], *finalisti[1]);
        const Echipa* invins = (campion == finalisti[0]) ? finalisti[1] : finalisti[0];
        notificaMeci(*campion, *invins, "Finala " + numeConferinta);
        return campion;
    }
    if (finalisti.size() == 1)
        return finalisti[0];

    return echipe.empty() ? nullptr : echipe[0];
}

// cppcheck-suppress unusedFunction
std::string SezonSimulator::simuleaza(const std::vector<Echipa>& echipe, const std::string& an) {
    notificaInceput(an);

    auto east = Stats::getEchipeConferinta(echipe, "East");
    auto west = Stats::getEchipeConferinta(echipe, "West");

    const Echipa* campionEast = simuleazaBracket(east, "East");
    const Echipa* campionWest = simuleazaBracket(west, "West");

    if (campionEast && campionWest) {
        const Echipa* campionNBA = Stats::simulateMeci(*campionEast, *campionWest);
        const Echipa* invins = (campionNBA == campionEast) ? campionWest : campionEast;
        notificaMeci(*campionNBA, *invins, "NBA Finals");
        notificaCampioana(*campionNBA);
        return campionNBA->getNume();
    }
    return "N/A";
}