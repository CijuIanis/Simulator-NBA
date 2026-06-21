#pragma once
#include <vector>
#include <string>
#include "Echipa.h"

namespace Gui {
    // deschide o fereastra cu logo-urile echipelor
    void afiseazaLogos(const std::vector<Echipa>& echipe, const std::string& titlu);
}