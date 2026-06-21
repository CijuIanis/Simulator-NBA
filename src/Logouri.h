#pragma once
#include <vector>
#include <string>
#include "Echipa.h"

namespace Gui {
    // fereastra cu logo-urile echipelor dintr-un sezon
    void afiseazaLogos(const std::vector<Echipa>& echipe, const std::string& titlu);

    // fereastra cu finala NBA si campioana (logo vs logo + campioana mare)
    void afiseazaCampioana(const std::vector<Echipa>& echipe, const std::string& an);

    // fereastra cu bracket-ul playoff-ului
    void afiseazaBracket(const std::vector<Echipa>& echipe, const std::string& an);

}