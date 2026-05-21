#pragma once
#include <vector>
#include <string>
#include "Echipa.h"
#include "Player.h"

namespace Stats {

    struct MediaStatistici {
        double mediaPPG;
        double mediaAPG;
        double mediaRPG;
        double mediaImpact;
    };

    struct RezultatComparatie {
        std::string numeEchipa1;
        std::string numeEchipa2;
        double scorImpact1;
        double scorImpact2;
        std::string castigatoare;
    };

    // Returneaza top N jucatori dupa impact score din toate echipele
    std::vector<const Player*> getTopJucatori(const std::vector<Echipa>& echipe, int n);

    // Returneaza top N jucatori All-Star dupa impact score
    std::vector<const Player*> getTopAllStars(const std::vector<Echipa>& echipe, int n);

    // Returneaza echipa cu cel mai mare scor de impact
    const Echipa* getEchipaMaxImpact(const std::vector<Echipa>& echipe);

    // Calculeaza media statisticilor din toate echipele unui sezon
    MediaStatistici getMediaStatistici(const std::vector<Echipa>& echipe);

    // Compara doua echipe si returneaza rezultatul
    RezultatComparatie compareEchipe(const Echipa& e1, const Echipa& e2);

}