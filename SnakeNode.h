#ifndef SNAKE_NODE_H
#define SNAKE_NODE_H

#include <SFML/Graphics.hpp>

namespace sfSnake {
class SnakeNode  //蛇块
{
   public:
    SnakeNode(sf::Vector2f position = sf::Vector2f(0, 0), double dx = 0,
              double dy = 0);
    // vector2f可以理解为装2个float的vec
    void setPosition(sf::Vector2f position);
    void setPosition(float x, float y);

    void move(float xOffset, float yOffset);

    void render(sf::RenderWindow& window, int i);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    static const float Width;
    static const float Height;
    

   private:
    // sf::RectangleShape shape_;
    sf::Sprite shape_;
    sf::Sprite shape_0;
    sf::Vector2f position_;
};
}  // namespace sfSnake

#endif