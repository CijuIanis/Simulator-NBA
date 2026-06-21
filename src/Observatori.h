#pragma once
#include <string>
#include <map>
#include <vector>
#include "Observer.h"
#include "Echipa.h"

// primul observator: scrie fiecare eveniment in consola
class LoggerConsola : public Observer {
public:
    void onInceputPlayoff(const std::string& an) override;
    void onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                const std::string& runda) override;
    void onCampioanaNBA(const Echipa& campioana) override;
};

// al doilea observator: numara cate meciuri a castigat fiecare echipa
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

// un meci din playoff retinut structurat (pentru bracket)
struct Meci {
    const Echipa* castigatoare;
    const Echipa* invinsa;
    std::string runda;
};

// al treilea observator: strange meciurile structurat, ca sa le putem pun in bracket
class ColectorMeciuri : public Observer {
private:
    std::vector<Meci> meciuri;

public:
    void onInceputPlayoff(const std::string& an) override;
    void onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                const std::string& runda) override;
    void onCampioanaNBA(const Echipa& campioana) override;

    const std::vector<Meci>& getMeciuri() const;
};