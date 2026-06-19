#include "Observatori.h"
#include <iostream>

// cppcheck-suppress unusedFunction
void LoggerConsola::onInceputPlayoff(const std::string& an) {
    std::cout << "[LOG] Incepe playoff-ul sezonului " << an << "\n";
}

// cppcheck-suppress unusedFunction
void LoggerConsola::onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                           const std::string& runda) {
    std::cout << "[LOG] " << runda << ": " << castigatoare.getNume()
              << " a invins " << invinsa.getNume() << "\n";
}

// cppcheck-suppress unusedFunction
void LoggerConsola::onCampioanaNBA(const Echipa& campioana) {
    std::cout << "[LOG] CAMPIOANA NBA: " << campioana.getNume()
              << " (logo: " << campioana.getLogoPath() << ")\n";
}

// cppcheck-suppress unusedFunction
void ColectorStatistici::onInceputPlayoff(const std::string& /*an*/) {
    victorii.clear();
    campioana.clear();
}

// cppcheck-suppress unusedFunction
void ColectorStatistici::onMeci(const Echipa& castigatoare, const Echipa& /*invinsa*/,
                                const std::string& /*runda*/) {
    victorii[castigatoare.getNume()]++;
}

// cppcheck-suppress unusedFunction
void ColectorStatistici::onCampioanaNBA(const Echipa& echipa) {
    campioana = echipa.getNume();
}

// cppcheck-suppress unusedFunction
void ColectorStatistici::afiseazaRaport() const {
    std::cout << "\n[STATISTICI] Victorii in playoff:\n";
    for (const auto& [nume, nr] : victorii)
        std::cout << "  " << nume << ": " << nr << "\n";
    if (!campioana.empty())
        std::cout << "[STATISTICI] Campioana: " << campioana << "\n";
}