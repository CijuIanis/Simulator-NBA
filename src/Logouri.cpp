#include "Logouri.h"
#include "Stats.h"
#include "SezonSimulator.h"
#include "Observatori.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>
#include <functional>
#include <cmath>
#include <cstdio>
#include <string>
#include <map>
#include <vector>

namespace Gui {

namespace {

const Echipa* gasesteEchipaDupaNume(const std::vector<Echipa>& echipe, const std::string& nume) {
    for (const auto& e : echipe)
        if (e.getNume() == nume)
            return &e;
    return nullptr;
}

void deseneazaLogo(sf::RenderWindow& window, const sf::Texture& tex,
                   float centerX, float topY, float zona) {
    sf::Sprite sprite(tex);
    const auto dim = tex.getSize();
    const float scara = zona / static_cast<float>(std::max(dim.x, dim.y));
    sprite.setScale({scara, scara});
    const float w = static_cast<float>(dim.x) * scara;
    sprite.setPosition({centerX - w / 2.0f, topY});
    window.draw(sprite);
}

void deseneazaTextCentrat(sf::RenderWindow& window, const sf::Font& font,
                          const std::string& sir, unsigned int marime,
                          float centerX, float topY, sf::Color culoare) {
    sf::Text text(font, sir, marime);
    text.setFillColor(culoare);
    const sf::FloatRect b = text.getLocalBounds();
    text.setPosition({centerX - b.size.x / 2.0f, topY});
    window.draw(text);
}

void deseneazaLinie(sf::RenderWindow& window, float x1, float y1, float x2, float y2, sf::Color c) {
    sf::Vertex linie[2] = {
        sf::Vertex{{x1, y1}, c},
        sf::Vertex{{x2, y2}, c}
    };
    window.draw(linie, 2, sf::PrimitiveType::Lines);
}

void deseneazaConector(sf::RenderWindow& window, float fxEdge, float fy0, float fy1,
                       float mxEdge, float my, sf::Color c) {
    const float midX = (fxEdge + mxEdge) / 2.0f;
    deseneazaLinie(window, fxEdge, fy0, midX, fy0, c);
    deseneazaLinie(window, fxEdge, fy1, midX, fy1, c);
    deseneazaLinie(window, midX, fy0, midX, fy1, c);
    deseneazaLinie(window, midX, my, mxEdge, my, c);
}

void deseneazaMatchup(sf::RenderWindow& window, const sf::Font& font, bool areFont,
                      const std::map<std::string, sf::Texture>& texturi,
                      float x, float y, float w, float h,
                      const Echipa* castigatoare, const Echipa* invinsa) {
    sf::RectangleShape box({w, h});
    box.setPosition({x, y});
    box.setFillColor(sf::Color(40, 42, 54));
    box.setOutlineThickness(1.0f);
    box.setOutlineColor(sf::Color(70, 72, 86));
    window.draw(box);

    const sf::Color auriu(255, 215, 0);
    const sf::Color gri(165, 165, 175);
    const float logoZona = h / 2.0f - 10.0f;

    auto deseneazaRand = [&](const Echipa* e, float ry, sf::Color culoare) {
        if (e == nullptr)
            return;
        const auto it = texturi.find(e->getNume());
        if (it != texturi.end())
            deseneazaLogo(window, it->second, x + 18.0f, ry + 4.0f, logoZona);
        if (areFont) {
            sf::Text text(font, e->getNume(), 14);
            text.setFillColor(culoare);
            text.setPosition({x + 38.0f, ry + 6.0f});
            window.draw(text);
        }
    };

    deseneazaRand(castigatoare, y, auriu);
    deseneazaRand(invinsa, y + h / 2.0f, gri);
}

}  // namespace

// cppcheck-suppress unusedFunction
void afiseazaLogos(const std::vector<Echipa>& echipe, const std::string& titlu) {
    const unsigned int latime = 960;
    const unsigned int inaltime = 800;
    sf::RenderWindow window(sf::VideoMode({latime, inaltime}), titlu);
    window.setFramerateLimit(60);

    sf::Font font;
    const bool areFont = font.openFromFile("assets/fonts/Mulish-Regular.ttf");
    if (!areFont)
        std::cerr << "Nu am gasit assets/fonts/Mulish-Regular.ttf; afisez doar logo-urile.\n";

    std::vector<sf::Texture> texturi;
    std::vector<bool> areLogo;
    texturi.reserve(echipe.size());
    areLogo.reserve(echipe.size());
    for (const auto& echipa : echipe) {
        sf::Texture tex;
        const bool ok = tex.loadFromFile(echipa.getLogoPath());
        texturi.push_back(std::move(tex));
        areLogo.push_back(ok);
    }

    const int coloane = 6;
    const float celula = 150.0f;
    const float margine = 20.0f;
    const float zonaLogo = celula - 2 * margine;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(28, 28, 38));

        for (std::size_t i = 0; i < echipe.size(); ++i) {
            const int col = static_cast<int>(i) % coloane;
            const int rand = static_cast<int>(i) / coloane;
            const float x = margine + static_cast<float>(col) * celula;
            const float y = margine + static_cast<float>(rand) * celula;
            const float centruX = x + zonaLogo / 2.0f;

            if (areLogo[i])
                deseneazaLogo(window, texturi[i], centruX, y, zonaLogo);

            if (areFont)
                deseneazaTextCentrat(window, font, echipe[i].getNume(), 13,
                                     centruX, y + zonaLogo + 6.0f, sf::Color(220, 220, 230));
        }

        window.display();
    }
}

// cppcheck-suppress unusedFunction
void afiseazaCampioana(const std::vector<Echipa>& echipe, const std::string& an) {
    const auto rez = Stats::simulatePlayoff(echipe);

    const Echipa* est = gasesteEchipaDupaNume(echipe, rez.campioanaEast);
    const Echipa* vest = gasesteEchipaDupaNume(echipe, rez.campioanaWest);
    const Echipa* campioana = gasesteEchipaDupaNume(echipe, rez.campioanaFinals);

    std::mt19937 rng(static_cast<unsigned int>(std::hash<std::string>{}(an)));
    std::uniform_int_distribution<int> distScor(100, 120);
    std::uniform_int_distribution<int> distMarja(3, 14);
    const int scorCampFinal = distScor(rng);
    const int scorAdvFinal = scorCampFinal - distMarja(rng);

    const bool estCampion = (est != nullptr && est == campioana);
    const bool vestCampion = (vest != nullptr && vest == campioana);
    const int targetEst = estCampion ? scorCampFinal : scorAdvFinal;
    const int targetVest = vestCampion ? scorCampFinal : scorAdvFinal;

    const unsigned int latime = 960;
    const unsigned int inaltime = 720;
    sf::RenderWindow window(sf::VideoMode({latime, inaltime}), "Finala NBA " + an);
    window.setFramerateLimit(60);

    sf::Font font;
    const bool areFont = font.openFromFile("assets/fonts/Mulish-Regular.ttf");

    sf::Texture texEst, texVest;
    const bool okEst = (est != nullptr) && texEst.loadFromFile(est->getLogoPath());
    const bool okVest = (vest != nullptr) && texVest.loadFromFile(vest->getLogoPath());

    const sf::Color auriu(255, 215, 0);
    const sf::Color alb(230, 230, 235);

    const float durata = 8.0f;
    sf::Clock ceas;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float t = ceas.getElapsedTime().asSeconds() / durata;
        if (t > 1.0f) t = 1.0f;
        const bool gata = (t >= 1.0f);

        const int scorEst = static_cast<int>(std::round(static_cast<float>(targetEst) * t));
        const int scorVest = static_cast<int>(std::round(static_cast<float>(targetVest) * t));

        const float minRamase = 48.0f * (1.0f - t);
        const int mm = static_cast<int>(minRamase);
        const int ss = static_cast<int>((minRamase - static_cast<float>(mm)) * 60.0f);

        window.clear(sf::Color(20, 22, 32));

        if (areFont)
            deseneazaTextCentrat(window, font, "Finala NBA " + an, 30, 480.0f, 26.0f, alb);

        if (okEst)  deseneazaLogo(window, texEst, 290.0f, 90.0f, 150.0f);
        if (okVest) deseneazaLogo(window, texVest, 670.0f, 90.0f, 150.0f);

        if (areFont) {
            if (est)  deseneazaTextCentrat(window, font, est->getNume(), 15, 290.0f, 248.0f,
                                           (estCampion && gata) ? auriu : alb);
            if (vest) deseneazaTextCentrat(window, font, vest->getNume(), 15, 670.0f, 248.0f,
                                           (vestCampion && gata) ? auriu : alb);

            deseneazaTextCentrat(window, font, std::to_string(scorEst), 46, 290.0f, 285.0f,
                                 (estCampion && gata) ? auriu : alb);
            deseneazaTextCentrat(window, font, std::to_string(scorVest), 46, 670.0f, 285.0f,
                                 (vestCampion && gata) ? auriu : alb);

            if (gata) {
                deseneazaTextCentrat(window, font, "FINAL", 34, 480.0f, 295.0f, auriu);
            } else {
                char buf[8];
                std::snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);
                deseneazaTextCentrat(window, font, buf, 34, 480.0f, 295.0f, alb);
            }
        }

        if (gata) {
            if (estCampion && okEst)
                deseneazaLogo(window, texEst, 480.0f, 345.0f, 170.0f);
            else if (vestCampion && okVest)
                deseneazaLogo(window, texVest, 480.0f, 345.0f, 170.0f);

            if (areFont && campioana)
                deseneazaTextCentrat(window, font, "CAMPIOANA NBA: " + campioana->getNume(),
                                     26, 480.0f, 530.0f, auriu);
        }

        window.display();
    }
}

// cppcheck-suppress unusedFunction
void afiseazaBracket(const std::vector<Echipa>& echipe, const std::string& an) {
    SezonSimulator simulator;
    ColectorMeciuri colector;
    simulator.adaugaObserver(&colector);
    simulator.simuleaza(echipe, an);
    const std::vector<Meci>& meciuri = colector.getMeciuri();

    auto filtreaza = [&](const std::string& runda) {
        std::vector<const Meci*> rez;
        for (const auto& m : meciuri)
            if (m.runda == runda)
                rez.push_back(&m);
        return rez;
    };
    const auto eastR1    = filtreaza("East R1");
    const auto eastSemi  = filtreaza("East Semifinale");
    const auto eastFinal = filtreaza("Finala East");
    const auto westR1    = filtreaza("West R1");
    const auto westSemi  = filtreaza("West Semifinale");
    const auto westFinal = filtreaza("Finala West");
    const auto nbaFinals = filtreaza("NBA Finals");

    std::map<std::string, sf::Texture> texturi;
    auto incarca = [&](const Echipa* e) {
        if (e == nullptr || texturi.count(e->getNume()) > 0)
            return;
        sf::Texture tex;
        if (tex.loadFromFile(e->getLogoPath()))
            texturi.emplace(e->getNume(), std::move(tex));
    };
    for (const auto& m : meciuri) {
        incarca(m.castigatoare);
        incarca(m.invinsa);
    }

    const unsigned int latime = 1400;
    const unsigned int inaltime = 720;
    sf::RenderWindow window(sf::VideoMode({latime, inaltime}), "Bracket Playoff " + an);
    window.setFramerateLimit(60);

    sf::Font font;
    const bool areFont = font.openFromFile("assets/fonts/Mulish-Regular.ttf");

    const float boxW = 195.0f;
    const float boxH = 64.0f;

    const float xEastR1 = 20.0f, xEastSemi = 230.0f, xEastFinal = 440.0f;
    const float xWestR1 = 1185.0f, xWestSemi = 975.0f, xWestFinal = 765.0f;
    const float xCentru = 700.0f;

    const sf::Color auriu(255, 215, 0);
    const sf::Color alb(230, 230, 235);
    const sf::Color linieCol(80, 82, 96);

    auto cy = [&](float y) { return y + boxH / 2.0f; };

    // pozitii y centrate uniform, in functie de cate meciuri are runda
    const float sus = 80.0f;
    const float disponibil = static_cast<float>(inaltime) - sus - 30.0f;
    auto yCentrate = [&](std::size_t n) {
        std::vector<float> ys;
        if (n == 0) return ys;
        const float pas = disponibil / static_cast<float>(n);
        for (std::size_t i = 0; i < n; ++i)
            ys.push_back(sus + pas * static_cast<float>(i) + (pas - boxH) / 2.0f);
        return ys;
    };

    const auto yEastR1   = yCentrate(eastR1.size());
    const auto yEastSemi = yCentrate(eastSemi.size());
    const auto yEastFin  = yCentrate(eastFinal.size());
    const auto yWestR1   = yCentrate(westR1.size());
    const auto yWestSemi = yCentrate(westSemi.size());
    const auto yWestFin  = yCentrate(westFinal.size());

    // conectori doar cand runda se injumatateste curat
    auto conectori = [&](const std::vector<float>& yFeeder, float xFeederEdge,
                          const std::vector<float>& yMerged, float xMergedEdge) {
        if (yFeeder.empty() || yMerged.empty())
            return;

        if (yFeeder.size() == 2 * yMerged.size()) {
            for (std::size_t i = 0; i < yMerged.size(); ++i)
                deseneazaConector(window, xFeederEdge, cy(yFeeder[2 * i]), cy(yFeeder[2 * i + 1]),
                                  xMergedEdge, cy(yMerged[i]), linieCol);
            return;
        }

        const float midX = (xFeederEdge + xMergedEdge) / 2.0f;
        float minY = cy(yFeeder[0]);
        float maxY = cy(yFeeder[0]);
        for (float y : yFeeder) { minY = std::min(minY, cy(y)); maxY = std::max(maxY, cy(y)); }
        for (float y : yMerged) { minY = std::min(minY, cy(y)); maxY = std::max(maxY, cy(y)); }

        deseneazaLinie(window, midX, minY, midX, maxY, linieCol);
        for (float y : yFeeder) deseneazaLinie(window, xFeederEdge, cy(y), midX, cy(y), linieCol);
        for (float y : yMerged) deseneazaLinie(window, midX, cy(y), xMergedEdge, cy(y), linieCol);
    };

    auto deseneazaColoana = [&](const std::vector<const Meci*>& runda, float x, const std::vector<float>& ys) {
        for (std::size_t i = 0; i < runda.size() && i < ys.size(); ++i)
            deseneazaMatchup(window, font, areFont, texturi, x, ys[i], boxW, boxH,
                             runda[i]->castigatoare, runda[i]->invinsa);
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(24, 26, 34));

        // conectori (sub cutii)
        conectori(yEastR1, xEastR1 + boxW, yEastSemi, xEastSemi);
        conectori(yEastSemi, xEastSemi + boxW, yEastFin, xEastFinal);
        conectori(yWestR1, xWestR1, yWestSemi, xWestSemi + boxW);
        conectori(yWestSemi, xWestSemi, yWestFin, xWestFinal + boxW);

        // cutiile de meci
        deseneazaColoana(eastR1, xEastR1, yEastR1);
        deseneazaColoana(eastSemi, xEastSemi, yEastSemi);
        deseneazaColoana(eastFinal, xEastFinal, yEastFin);
        deseneazaColoana(westR1, xWestR1, yWestR1);
        deseneazaColoana(westSemi, xWestSemi, yWestSemi);
        deseneazaColoana(westFinal, xWestFinal, yWestFin);

        // campioana, centrata vertical
        const float midY = static_cast<float>(inaltime) / 2.0f;
        const Echipa* campion = nbaFinals.empty() ? nullptr : nbaFinals[0]->castigatoare;
        if (areFont)
            deseneazaTextCentrat(window, font, "CAMPIOANA", 22, xCentru, midY - 95.0f, auriu);
        if (campion != nullptr) {
            const auto it = texturi.find(campion->getNume());
            if (it != texturi.end())
                deseneazaLogo(window, it->second, xCentru, midY - 60.0f, 110.0f);
            if (areFont)
                deseneazaTextCentrat(window, font, campion->getNume(), 18, xCentru, midY + 65.0f, auriu);
        }

        if (areFont)
            deseneazaTextCentrat(window, font, "Playoff " + an, 26, xCentru, 20.0f, alb);

        window.display();
    }
}
}