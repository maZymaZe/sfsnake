#ifndef GAME_H
#define GAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Screen.h"

namespace sfSnake {
class Game {
   public:
    Game();

    void run();

    void handleInput();
    void update(sf::Time delta);
    void render();

    static const int Width = 1280;
    static const int Height = 960;
    //自然是屏幕大小设置，需要某种整除关系
    static std::shared_ptr<Screen> Screen;

    static bool ingame;
    static bool pause;
    static int speedup;

   private:
    sf::RenderWindow window_;
    sf::Music bgMusic_;
    static const sf::Time TimePerFrame;
};
}  // namespace sfSnake
#endif