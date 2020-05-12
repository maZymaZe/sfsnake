#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <deque>

#include "Fruit.h"
#include "SnakeNode.h"

namespace sfSnake {
class Snake {
   public:
    Snake();

    void handleInput();
    void update(sf::Time delta);
    void render(sf::RenderWindow& window);

    void checkFruitCollisions(std::vector<Fruit>& fruits);
    bool ok(int x, int y);
    int pile=0;

    bool hitSelf() const;
    unsigned getSize() const;

   private:
    void move();
    void checkEdgeCollisions();  //碰边
    void checkSelfCollisions();  //碰自己
    void initNodes();

    bool hitSelf_;

    sf::Vector2f position_;
    double dx, dy;

    sf::SoundBuffer pickupBuffer_;
    sf::Sound pickupSound_;

    sf::SoundBuffer dieBuffer_;
    sf::Sound dieSound_;

    std::deque<SnakeNode> nodes_;
    //蛇身记录，第0是头，最后是尾

    static const int InitialSize;
};
}  // namespace sfSnake

#endif