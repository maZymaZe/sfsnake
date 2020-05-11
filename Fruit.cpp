#include "Fruit.h"

#include <SFML/Graphics.hpp>

using namespace sfSnake;

const float Fruit::Radius = 5.0f;  //设置大小
const int foodsize[8] = {1, 1, 1, 1, 2, 2, 3, 3};
const int gr[8] = {0, 0, 1, 1, 2, 2, 3, 3};
const sf::Color foodcolor[8] = {
    sf::Color::Black, sf::Color(150, 75, 0), sf::Color::Green, sf::Color::Green,
    sf::Color::Blue,  sf::Color::Blue,       sf::Color::Red,   sf::Color::Red};
Fruit::Fruit(sf::Vector2f position, int foodmode) {
    sz = gr[foodmode];
    shape_.setPosition(position);
    shape_.setRadius(Fruit::Radius * foodsize[foodmode]);
    shape_.setFillColor(foodcolor[foodmode]);
    shape_.setOutlineColor(sf::Color::Magenta);
    shape_.setOutlineThickness(-1.f);
}

void Fruit::render(sf::RenderWindow& window) { window.draw(shape_); }

sf::FloatRect Fruit::getBounds() const {
    return shape_.getGlobalBounds();  //取位置
}