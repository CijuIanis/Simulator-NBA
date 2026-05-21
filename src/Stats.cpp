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

}