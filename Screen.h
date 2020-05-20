#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>

class Screen {
    //屏幕母类
   public:
    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(sf::Time delta) = 0;
    virtual void render(sf::RenderWindow& window) = 0;  //渲染

    static int gridcolor;
    static int bg;
};

#endif