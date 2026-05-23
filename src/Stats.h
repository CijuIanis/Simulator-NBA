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

    struct RezultatPlayoff {
        std::string campioanaEast;
        std::string campioanaWest;
        std::string campioanaFinals;
        std::vector<std::string> rezultateRunde;
    };

    std::vector<const Player*> getTopJucatori(const std::vector<Echipa>& echipe, int n);
    std::vector<const Player*> getTopAllStars(const std::vector<Echipa>& echipe, int n);
    const Echipa* getEchipaMaxImpact(const std::vector<Echipa>& echipe);
    const Echipa* simulateMeci(const Echipa& e1, const Echipa& e2);
    std::vector<const Echipa*> getEchipeConferinta(const std::vector<Echipa>& echipe, const std::string& conferinta);
    MediaStatistici getMediaStatistici(const std::vector<Echipa>& echipe);
    RezultatComparatie compareEchipe(const Echipa& e1, const Echipa& e2);

    // Simuleaza playoff-ul NBA complet (top 8 East vs top 8 West)
    RezultatPlayoff simulatePlayoff(const std::vector<Echipa>& echipe);

}