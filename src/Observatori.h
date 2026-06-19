#pragma once
#include <string>
#include <map>
#include "Observer.h"
#include "Echipa.h"

// primul obervator scrie fiecare eveniment in consola
class LoggerConsola : public Observer {
public:
    void onInceputPlayoff(const std::string& an) override;
    void onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                const std::string& runda) override;
    void onCampioanaNBA(const Echipa& campioana) override;
};

// al doilea observator numara cate meciuri a castigat fiecare echipa in playoff
class ColectorStatistici : public Observer {
private:
    std::map<std::string, int> victorii;
    std::string campioana;

public:
    void onInceputPlayoff(const std::string& an) override;
    void onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                const std::string& runda) override;
    void onCampioanaNBA(const Echipa& echipa) override;

    void afiseazaRaport() const;
};