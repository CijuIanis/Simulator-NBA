#include "Stats.h"
#include <algorithm>

namespace Stats {

    // cppcheck-suppress unusedFunction
    std::vector<const Player*> getTopJucatori(const std::vector<Echipa>& echipe, int n) {
        std::vector<const Player*> toti;
        for (const auto& echipa : echipe)
            for (const auto& player : echipa.getRoster())
                toti.push_back(player.get());

        std::sort(toti.begin(), toti.end(), [](const Player* a, const Player* b) {
            return a->getImpactScore() > b->getImpactScore();
        });

        if (n > static_cast<int>(toti.size()))
            n = static_cast<int>(toti.size());

        return std::vector<const Player*>(toti.begin(), toti.begin() + n);
    }

    // cppcheck-suppress unusedFunction
    std::vector<const Player*> getTopAllStars(const std::vector<Echipa>& echipe, int n) {
        std::vector<const Player*> allStars;
        for (const auto& echipa : echipe)
            for (const auto& player : echipa.getRoster())
                if (player->isAllStar())
                    allStars.push_back(player.get());

        std::sort(allStars.begin(), allStars.end(), [](const Player* a, const Player* b) {
            return a->getImpactScore() > b->getImpactScore();
        });

        if (n > static_cast<int>(allStars.size()))
            n = static_cast<int>(allStars.size());

        return std::vector<const Player*>(allStars.begin(), allStars.begin() + n);
    }

    // cppcheck-suppress unusedFunction
    const Echipa* getEchipaMaxImpact(const std::vector<Echipa>& echipe) {
        if (echipe.empty())
            return nullptr;

        const Echipa* best = &echipe[0];
        for (const auto& echipa : echipe)
            if (echipa.getScorImpact() > best->getScorImpact())
                best = &echipa;
        return best;
    }

    // cppcheck-suppress unusedFunction
    const Echipa* simulateMeci(const Echipa& e1, const Echipa& e2) {
        if (e1.getScorImpact() >= e2.getScorImpact())
            return &e1;
        return &e2;
    }

    // cppcheck-suppress unusedFunction
    std::vector<const Echipa*> getEchipeConferinta(const std::vector<Echipa>& echipe, const std::string& conferinta) {
        std::vector<const Echipa*> rezultat;
        for (const auto& echipa : echipe)
            if (echipa.getConferinta() == conferinta)
                rezultat.push_back(&echipa);
        return rezultat;
    }

    // cppcheck-suppress unusedFunction
    MediaStatistici getMediaStatistici(const std::vector<Echipa>& echipe) {
        double totalPPG = 0.0, totalAPG = 0.0, totalRPG = 0.0, totalImpact = 0.0;
        int nrJucatori = 0;

        for (const auto& echipa : echipe) {
            for (const auto& player : echipa.getRoster()) {
                totalPPG += player->getPointsPerGame();
                totalAPG += player->getAssistsPerGame();
                totalRPG += player->getReboundsPerGame();
                totalImpact += player->getImpactScore();
                nrJucatori++;
            }
        }

        if (nrJucatori == 0)
            return {0.0, 0.0, 0.0, 0.0};

        return {
            totalPPG / nrJucatori,
            totalAPG / nrJucatori,
            totalRPG / nrJucatori,
            totalImpact / nrJucatori
        };
    }

    // cppcheck-suppress unusedFunction
    RezultatComparatie compareEchipe(const Echipa& e1, const Echipa& e2) {
        double scor1 = e1.getScorImpact();
        double scor2 = e2.getScorImpact();

        std::string castigatoare;
        if (scor1 > scor2)
            castigatoare = e1.getNume();
        else if (scor2 > scor1)
            castigatoare = e2.getNume();
        else
            castigatoare = "Egalitate";

        return {e1.getNume(), e2.getNume(), scor1, scor2, castigatoare};
    }

    static std::vector<const Echipa*> sortEchipeDupaImpact(std::vector<const Echipa*> echipe) {
        std::sort(echipe.begin(), echipe.end(), [](const Echipa* a, const Echipa* b) {
            return a->getScorImpact() > b->getScorImpact();
        });
        return echipe;
    }

    static const Echipa* simulateBracket(std::vector<const Echipa*> echipe,
                                          std::vector<std::string>& rezultate,
                                          const std::string& numeConferinta) {
        // Top 8 echipe
        if (static_cast<int>(echipe.size()) > 8)
            echipe.resize(8);

        rezultate.push_back("=== " + numeConferinta + " ===");

        // Runda 1: 1vs8, 2vs7, 3vs6, 4vs5
        std::vector<const Echipa*> semifinalisti;
        std::vector<std::pair<int,int>> perechi = {{0,7},{1,6},{2,5},{3,4}};
        for (const auto& [i, j] : perechi) {
            if (i < static_cast<int>(echipe.size()) && j < static_cast<int>(echipe.size())) {
                const Echipa* castig = simulateMeci(*echipe[i], *echipe[j]);
                semifinalisti.push_back(castig);
                rezultate.push_back("R1: " + echipe[i]->getNume() + " vs " +
                                    echipe[j]->getNume() + " -> " + castig->getNume());
            } else if (i < static_cast<int>(echipe.size())) {
                semifinalisti.push_back(echipe[i]);
                rezultate.push_back("R1: " + echipe[i]->getNume() + " - bye");
            }
        }

        // Runda 2: semifinale conferinta
        std::vector<const Echipa*> finalisti;
        for (int i = 0; i + 1 < static_cast<int>(semifinalisti.size()); i += 2) {
            const Echipa* castig = simulateMeci(*semifinalisti[i], *semifinalisti[i+1]);
            finalisti.push_back(castig);
            rezultate.push_back("R2: " + semifinalisti[i]->getNume() + " vs " +
                                semifinalisti[i+1]->getNume() + " -> " + castig->getNume());
        }

        // Finala conferinta
        if (finalisti.size() >= 2) {
            const Echipa* campion = simulateMeci(*finalisti[0], *finalisti[1]);
            rezultate.push_back("Finala " + numeConferinta + ": " +
                                finalisti[0]->getNume() + " vs " +
                                finalisti[1]->getNume() + " -> " + campion->getNume());
            return campion;
        } else if (finalisti.size() == 1) {
            return finalisti[0];
        }

        return echipe.empty() ? nullptr : echipe[0];
    }

    // cppcheck-suppress unusedFunction
    RezultatPlayoff simulatePlayoff(const std::vector<Echipa>& echipe) {
        RezultatPlayoff rezultat;

        // Separa echipele pe conferinte si sorteaza dupa impact
        auto echipeEast = sortEchipeDupaImpact(getEchipeConferinta(echipe, "East"));
        auto echipeWest = sortEchipeDupaImpact(getEchipeConferinta(echipe, "West"));

        // Simuleaza bracket East
        const Echipa* campionEast = simulateBracket(echipeEast, rezultat.rezultateRunde, "East");
        rezultat.campioanaEast = campionEast ? campionEast->getNume() : "N/A";

        // Simuleaza bracket West
        const Echipa* campionWest = simulateBracket(echipeWest, rezultat.rezultateRunde, "West");
        rezultat.campioanaWest = campionWest ? campionWest->getNume() : "N/A";

        // NBA Finals
        if (campionEast && campionWest) {
            const Echipa* campionNBA = simulateMeci(*campionEast, *campionWest);
            rezultat.campioanaFinals = campionNBA->getNume();
            rezultat.rezultateRunde.push_back("=== NBA FINALS ===");
            rezultat.rezultateRunde.push_back("Finals: " + campionEast->getNume() +
                                              " vs " + campionWest->getNume() +
                                              " -> " + campionNBA->getNume() + " CAMPION!");
        }

        return rezultat;
    }

}