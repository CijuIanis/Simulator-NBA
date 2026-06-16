#pragma once
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <algorithm>
#include "Player.h"
#include "Echipa.h"

namespace Stats {

    // lider la o statistica de tip T.
    // primeste o functie "extractor" care spune ce statistica se masoara.
    template <typename T>
    class StatLeader {
        std::string numeStatistica;
        std::function<T(const Player&)> extractor;

    public:
        StatLeader(std::string nume, std::function<T(const Player&)> ext)
            : numeStatistica(std::move(nume)), extractor(std::move(ext)) {}

        const std::string& getNumeStatistica() const { return numeStatistica; }

        T getValoare(const Player& player) const {
            return extractor(player);
        }

        // cel mai bun jucator la statistica respectiva
        const Player* getLider(const std::vector<Echipa>& echipe) const {
            const Player* lider = nullptr;
            T valoareMax{};
            for (const auto& echipa : echipe) {
                for (const auto& player : echipa.getRoster()) {
                    T valoare = extractor(*player);
                    if (lider == nullptr || valoare > valoareMax) {
                        lider = player.get();
                        valoareMax = valoare;
                    }
                }
            }
            return lider;
        }

        // top N jucatori la statistica respectiva, impreuna cu valoarea lor
        std::vector<std::pair<const Player*, T>>
        getTopN(const std::vector<Echipa>& echipe, int n) const {
            std::vector<std::pair<const Player*, T>> toti;
            for (const auto& echipa : echipe)
                for (const auto& player : echipa.getRoster())
                    toti.emplace_back(player.get(), extractor(*player));

            std::sort(toti.begin(), toti.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });

            if (n > static_cast<int>(toti.size()))
                n = static_cast<int>(toti.size());

            return std::vector<std::pair<const Player*, T>>(toti.begin(), toti.begin() + n);
        }
    };
}