#pragma once
#include <string>
#include <vector>
#include "Echipa.h"
#include "Observer.h"

class SezonSimulator {
private:
    std::vector<Observer*> observatori;

    void notificaInceput(const std::string& an);
    void notificaMeci(const Echipa& castigatoare, const Echipa& invinsa,
                      const std::string& runda);
    void notificaCampioana(const Echipa& campioana);

    const Echipa* simuleazaBracket(std::vector<const Echipa*> echipe,
                                   const std::string& numeConferinta);

public:
    void adaugaObserver(Observer* obs);
    // ruleaza playoff-ul, emite evenimente si returneaza campioana
    std::string simuleaza(const std::vector<Echipa>& echipe, const std::string& an);
};