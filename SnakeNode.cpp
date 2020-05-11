#include "SnakeNode.h"

#include <SFML/Graphics.hpp>

using namespace sfSnake;

const float SnakeNode::Width = 10.f;
const float SnakeNode::Height = 10.f;
//蛇块大小参数设置

SnakeNode::SnakeNode(sf::Vector2f position) : position_(position) {
    shape_.setPosition(position_);
    shape_.setFillColor(sf::Color::Green);
    shape_.setSize(sf::Vector2f(SnakeNode::Width, SnakeNode::Height));
    shape_.setOutlineColor(sf::Color::White);
    shape_.setOutlineThickness(-1.f);

    // shape的参数设置
}

void SnakeNode::setPosition(sf::Vector2f position) {
    position_ = position;
    shape_.setPosition(position_);
}
//由下面重载到上面
void SnakeNode::setPosition(float x, float y) {
    position_.x = x;
    position_.y = y;
    shape_.setPosition(position_);
}

void SnakeNode::move(float xOffset, float yOffset) {
    position_.x += xOffset;
    position_.y += yOffset;
    shape_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const {
    return shape_.getGlobalBounds();
    //不太清楚
}

sf::Vector2f SnakeNode::getPosition() const { return position_; }

void SnakeNode::render(sf::RenderWindow& window) { window.draw(shape_); }