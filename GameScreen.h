#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>

#include "Fruit.h"
#include "Screen.h"
#include "Snake.h"

namespace sfSnake {
class GameScreen : public Screen {
   public:
    GameScreen();

    void handleInput(sf::RenderWindow& window) override;
    void update(sf::Time delta) override;
    void render(sf::RenderWindow& window) override;

    void generateFruit();

   private:
    Snake snake_;
    std::vector<Fruit> fruit_;
};
}  // namespace sfSnake
void drawyline(sf::RenderWindow& window);
void drawxline(sf::RenderWindow& window);
#endif