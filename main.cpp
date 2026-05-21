#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <tabulate/table.hpp>
#include "Sezon.h"
#include "SezonLoader.h"
#include "Utilitati.h"
#include "Exceptii.h"
#include "Guard.h"
#include "Forward.h"
#include "Center.h"
#include "Stats.h"
#include "TwoWayPlayer.h"

std::string formatNum(double val) {
    std::ostringstream oss;
    double decimala = val - static_cast<int>(val);
    if (decimala < 0.05 && decimala > -0.05)
        oss << static_cast<int>(val);
    else
        oss << std::fixed << std::setprecision(1) << val;
    return oss.str();
}

int main() {
    std::vector<std::string> fisiere;
    for (const auto& entry : std::filesystem::directory_iterator("data/seasons/")) {
        if (entry.path().extension() == ".json") {
            fisiere.push_back(entry.path().string());
        }
    }
    std::sort(fisiere.begin(), fisiere.end());

    std::vector<Sezon> season;
    for (const auto& fisier : fisiere) {
        try {
            season.push_back(SezonLoader::incarcaDinFisier(fisier));
        } catch (const FisierException& e) {
            std::cerr << "[SKIP] " << e.what() << "\n";
        } catch (const JucatorException& e) {
            std::cerr << "[SKIP] " << e.what() << "\n";
        }
    }

    while (true) {
        std::cout << "\n=== Simulator NBA ===\n";
        std::cout << "Sezoane disponibile:\n";
        for (auto i = 0u; i < season.size(); i++)
            std::cout << "  " << season[i].getAn() << "\n";
        std::cout << "\nIntroduceti comanda [anul sezonului (ex:'2022-23') / 'compara' / 'exit']: ";

        std::string input;
        std::cin >> input;

        if (input == "exit") break;

        if (input == "compara") {
            std::cout << "Introduceti anul sezonului: ";
            std::string an;
            std::cin >> an;
            std::cin.ignore();

            const Sezon* sezonGasit = nullptr;
            for (const auto& sezon : season) {
                if (sezon.getAn() == an) {
                    sezonGasit = &sezon;
                    break;
                }
            }

            if (!sezonGasit) {
                std::cerr << "Sezonul '" << an << "' nu a fost gasit!\n";
                continue;
            }

            std::cout << "Introduceti numele primului jucator: ";
            std::string nume1;
            std::getline(std::cin, nume1);

            std::cout << "Introduceti numele celui de-al doilea jucator: ";
            std::string nume2;
            std::getline(std::cin, nume2);

            const Player* j1 = sezonGasit->gasesteJucator(nume1);
            const Player* j2 = sezonGasit->gasesteJucator(nume2);

            if (!j1) {
                std::cerr << "Jucatorul '" << nume1 << "' nu a fost gasit!\n";
                continue;
            }
            if (!j2) {
                std::cerr << "Jucatorul '" << nume2 << "' nu a fost gasit!\n";
                continue;
            }

            tabulate::Table tabel;
            tabel.add_row({"Statistica", j1->getName(), j2->getName()});
            tabel.add_row({"Pozitie", j1->getPosition(), j2->getPosition()});
            tabel.add_row({"PPG", formatNum(j1->getPointsPerGame()), formatNum(j2->getPointsPerGame())});
            tabel.add_row({"APG", formatNum(j1->getAssistsPerGame()), formatNum(j2->getAssistsPerGame())});
            tabel.add_row({"RPG", formatNum(j1->getReboundsPerGame()), formatNum(j2->getReboundsPerGame())});
            tabel.add_row({"Impact Score", formatNum(j1->getImpactScore()), formatNum(j2->getImpactScore())});
            tabel.add_row({"Role Score", formatNum(j1->calculateRoleScore()), formatNum(j2->calculateRoleScore())});
            tabel.add_row({"All-Star", j1->isAllStar() ? "DA" : "NU", j2->isAllStar() ? "DA" : "NU"});

            tabel[0].format()
                .font_style({tabulate::FontStyle::bold})
                .font_align(tabulate::FontAlign::center)
                .font_color(tabulate::Color::yellow);

            std::cout << "\n" << tabel << "\n";

            if (j1->getImpactScore() > j2->getImpactScore())
                std::cout << "Castigatorul: " << j1->getName() << "\n";
            else if (j2->getImpactScore() > j1->getImpactScore())
                std::cout << "Castigatorul: " << j2->getName() << "\n";
            else
                std::cout << "Egalitate!\n";

            continue;
        }

        try {
            bool gasit = false;
            for (const auto& sezon : season) {
                if (sezon.getAn() == input) {
                    gasit = true;
                    std::cout << "\n" << sezon << "\n";
                    sezon.afiseazaClassament();

                    const Player& best = sezon.getCelMaiBunJucatorDinSezon();
                    std::cout << "\nCel mai bun jucator: " << best.getName() << "\n";
                    std::cout << "  Role Score: " << formatNum(best.calculateRoleScore()) << "\n";

                    std::cout << "Favorita la titlu: "
                              << sezon.getEchipaFavorita().getNume() << "\n";

                    auto conferinte = sezon.getEchipeDupaConferinta();
                    std::cout << "\nEchipe dupa conferinta:\n";
                    for (const auto& [conf, echipaList] : conferinte)
                        std::cout << "  " << conf << ": " << echipaList.size() << " echipe\n";

                    break;
                }
            }
            if (!gasit)
                throw SezonException(input);
        } catch (const SezonException& e) {
            std::cerr << e.what() << "\n";
        }
    }

    std::cout << "\n--- Test Contract ---\n";
    Contract c1(30.14, 1, "MAX");
    Contract c2 = c1;
    Contract c3(1.0, 1, "ROOKIE");
    c3 = c1;
    std::cout << "Tip: " << c1.getType() << "\n";
    std::cout << "Durata: " << c1.getDurationYears() << " ani\n";
    std::cout << "Este MAX: " << c1.isMaxContract() << "\n";
    std::cout << "Valoare ramasa: " << c1.getRemainingValue(0) << "\n";
    std::cout << "Salariu/an: " << c1.getSalaryPerYear() << "\n";

    std::cout << "\n--- Test Player ---\n";
    Guard testGuard("Test Player", 25, "PG", 20.0, 5.0, 5.0, c1, 0.38);
    std::cout << "Pozitie: " << testGuard.getPosition() << "\n";
    std::cout << "Puncte: " << testGuard.getPointsPerGame() << "\n";
    std::cout << "Recuperari: " << testGuard.getReboundsPerGame() << "\n";
    std::cout << "Impact score: " << testGuard.getImpactScore() << "\n";
    std::cout << "Contract: " << testGuard.getContract() << "\n";
    std::cout << "Role Score: " << testGuard.calculateRoleScore() << "\n";
    std::cout << "Seniority: " << testGuard.getSeniority() << "\n";
    std::cout << "Contract expiring: " << c1.isExpiring() << "\n";


    std::cout << "\n--- Test comparare jucatori ---\n";
    Guard testGuard2("Test Player 2", 28, "SG", 25.0, 3.0, 4.0, c1, 0.42);
    if (testGuard2.getImpactScore() > testGuard.getImpactScore())
        std::cout << testGuard2.getName() << " e mai bun decat " << testGuard.getName() << "\n";
    else
        std::cout << testGuard.getName() << " e mai bun decat " << testGuard2.getName() << "\n";

    std::cout << "\n--- Test Echipa ---\n";
    if (!season.empty() && !season[0].getEchipe().empty()) {
        const Echipa& e = season[0].getEchipe()[0];
        std::cout << "Oras: " << e.getOras() << "\n";
        std::cout << "Salary Cap: " << e.getSalaryCap() << "\n";
        std::cout << "Nr jucatori: " << e.getRoster().size() << "\n";
        std::cout << "Contracte MAX: " << e.getNrJucatoriMaxContract() << "\n";
    }

    std::cout << "\n--- Test Sezon ---\n";
    if (!season.empty()) {
        std::cout << "Nr echipe: " << season[0].getNrEchipe() << "\n";
        auto conferinte = season[0].getEchipeDupaConferinta();
        for (const auto& [conf, echipaList] : conferinte)
            std::cout << conf << ": " << echipaList.size() << " echipe\n";
    }
    std::cout << "Varsta: " << testGuard.getAge() << "\n";
    std::cout << "Pase: " << testGuard.getAssistsPerGame() << "\n";

    if (!season.empty()) {
        std::cout << "Campioana: " << season[0].getCampioana() << "\n";
    }
    if (!season.empty() && !season[0].getEchipe().empty()) {
        std::cout << "Conferinta: " << season[0].getEchipe()[0].getConferinta() << "\n";
    }

    std::cout << "\n--- Test Utilitati ---\n";
    std::cout << Utilitati::formatSalary(30.14) << "\n";
    std::cout << Utilitati::formatStatLine(30.4, 4.3, 6.6) << "\n";
    std::cout << "Pozitie valida: " << Utilitati::isValidPosition("PG") << "\n";
    std::cout << "Sezon valid: " << Utilitati::isValidSeason("1995-96") << "\n";
    std::cout << "Salariu valid: " << Utilitati::isValidSalary(30.14) << "\n";
    std::cout << "Tip contract valid: " << Utilitati::isValidContractType("MAX") << "\n";

    std::cout << "\n--- Test Stats ---\n";
    if (!season.empty()) {
        auto topAllStars = Stats::getTopAllStars(season[0].getEchipe(), 3);
        std::cout << "Top 3 All-Stars:\n";
        for (const auto* jucator : topAllStars)
            std::cout << "  " << jucator->getName() << " - " << formatNum(jucator->getImpactScore()) << "\n";

        const Echipa* bestEchipa = Stats::getEchipaMaxImpact(season[0].getEchipe());
        if (bestEchipa)
            std::cout << "Echipa cu cel mai mare impact: " << bestEchipa->getNume() << "\n";

        auto top5 = Stats::getTopJucatori(season[0].getEchipe(), 5);
        std::cout << "Top 5 jucatori:\n";
        for (const auto* jucator : top5)
            std::cout << "  " << jucator->getName() << " - " << formatNum(jucator->getImpactScore()) << "\n";

        auto media = Stats::getMediaStatistici(season[0].getEchipe());
        std::cout << "Media PPG: " << formatNum(media.mediaPPG) << "\n";
        std::cout << "Media APG: " << formatNum(media.mediaAPG) << "\n";
        std::cout << "Media RPG: " << formatNum(media.mediaRPG) << "\n";

        if (season[0].getEchipe().size() >= 2) {
            auto rez = Stats::compareEchipe(season[0].getEchipe()[0], season[0].getEchipe()[1]);
            std::cout << "Castigatoare: " << rez.castigatoare << "\n";
        }
    }

    std::cout << "\n--- Test functii noi ---\n";
    if (!season.empty()) {
        const Player* bestGuard = season[0].getCelMaiBunGuard();
        if (bestGuard)
            std::cout << "Cel mai bun Guard: " << bestGuard->getName() << "\n";

        const Player* bestForward = season[0].getCelMaiBunForward();
        if (bestForward)
            std::cout << "Cel mai bun Forward: " << bestForward->getName() << "\n";

        const Player* bestCenter = season[0].getCelMaiBunCenter();
        if (bestCenter)
            std::cout << "Cel mai bun Center: " << bestCenter->getName() << "\n";

        if (!season[0].getEchipe().empty()) {
            const Echipa& echipa = season[0].getEchipe()[0];
            auto allStars = echipa.getJucatoriAllStar();
            std::cout << "All-Stars " << echipa.getNume() << ": " << allStars.size() << "\n";
            std::cout << "Valoare roster: $" << echipa.getValoareRoster() << "M\n";
        }
    }

    std::cout << "\n--- Test TwoWayPlayer ---\n";
    TwoWayPlayer tw("Kawhi Leonard", 28, "SF", 26.6, 3.3, 6.9, c1, 8.5);
    std::cout << tw << "\n";
    std::cout << "Role Score: " << tw.calculateRoleScore() << "\n";
    std::cout << "Defensive Rating: " << tw.getDefensiveRating() << "\n";

    std::cout << "\n--- Test Exceptii ---\n";
    try {
        throw JucatorException("Test Player", "pozitie invalida: XX");
    } catch (const NBAException& e) {
        std::cout << "NBAException prins: " << e.what() << "\n";
    }

    try {
        throw FisierException("fisier_inexistent.json");
    } catch (const NBAException& e) {
        std::cout << "NBAException prins: " << e.what() << "\n";
    }

    return 0;
}