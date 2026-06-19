#pragma once
#include <string>
#include "Echipa.h"

// interfata: orice clasa care vrea simularea o implementeaza
class Observer {
public:
    virtual ~Observer() = default;

    virtual void onInceputPlayoff(const std::string& an) = 0;
    virtual void onMeci(const Echipa& castigatoare, const Echipa& invinsa,
                        const std::string& runda) = 0;
    virtual void onCampioanaNBA(const Echipa& campioana) = 0;
};