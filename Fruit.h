#ifndef FRUIT_H
#define FRUIT_H

#include <SFML/Graphics.hpp>

namespace sfSnake {
class Fruit {
   public:
    Fruit(sf::Vector2f position = sf::Vector2f(0, 0), int foodmode = 0);

    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    int sz;

   private:
    sf::CircleShape shape_;  //圆形设置

    static const float Radius;  //半径
};
}  // namespace sfSnake

#endif