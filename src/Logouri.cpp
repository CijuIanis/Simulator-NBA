#include "Logouri.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>
#include <cstddef>

namespace Gui {

    // cppcheck-suppress unusedFunction
    void afiseazaLogos(const std::vector<Echipa>& echipe, const std::string& titlu) {
        const unsigned int latime = 960;
        const unsigned int inaltime = 720;
        sf::RenderWindow window(sf::VideoMode({latime, inaltime}), titlu);
        window.setFramerateLimit(60);

        // incarc texturile logo-urilor o singura data
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

        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }

            window.clear(sf::Color(28, 28, 38));

            for (std::size_t i = 0; i < echipe.size(); ++i) {
                if (!areLogo[i])
                    continue;

                sf::Sprite sprite(texturi[i]);

                // aranjez logo-ul ca sa incapa in celula
                const auto dim = texturi[i].getSize();
                const float maxLatura = static_cast<float>(std::max(dim.x, dim.y));
                const float scara = (celula - 2 * margine) / maxLatura;
                sprite.setScale({scara, scara});

                const float x = margine + (static_cast<float>(i) - static_cast<float>(static_cast<int>(i) / coloane) * coloane) * celula;
                const float y = margine + static_cast<float>(static_cast<int>(i) / coloane) * celula;
                sprite.setPosition({x, y});

                window.draw(sprite);
            }

            window.display();
        }
    }

}