#include "Snake.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Fruit.h"
#include "Game.h"
#include "GameOverScreen.h"
#include "SnakeNode.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;  //初始长度

Snake::Snake() : direction_(Direction::Up), hitSelf_(false) {
    initNodes();

    pickupBuffer_.loadFromFile("Sounds/pickup.aiff");
    pickupSound_.setBuffer(pickupBuffer_);
    pickupSound_.setVolume(30);

    dieBuffer_.loadFromFile("Sounds/die.wav");
    dieSound_.setBuffer(dieBuffer_);
    dieSound_.setVolume(50);
    //音效设置
}

void Snake::initNodes() {  //初始化
    for (int i = 0; i < Snake::InitialSize; ++i) {
        nodes_.push_back(
            SnakeNode(sf::Vector2f(Game::Width / 2 - SnakeNode::Width / 2,
                                   Game::Height / 2 - (SnakeNode::Height / 2) +
                                       (SnakeNode::Height * i))));
    }
}

void Snake::handleInput() {  //改方向：此处要排除180°转向
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        direction_ = Direction::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        direction_ = Direction::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        direction_ = Direction::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        direction_ = Direction::Right;
}

void Snake::update(sf::Time delta) {  //刷新
    move();
    checkEdgeCollisions();
    checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits) {
    decltype(fruits.begin()) toRemove = fruits.end();

    for (auto it = fruits.begin(); it != fruits.end(); ++it) {
        if (it->getBounds().intersects(nodes_[0].getBounds())) toRemove = it;
        //可能是接近到一定程度给吃
    }

    if (toRemove != fruits.end()) {
        pickupSound_.play();
        fruits.erase(toRemove);
        return;
        //（吃到处理：不去尾，没吃到去尾）
    } else
        nodes_.pop_back();
}

unsigned Snake::getSize() const { return nodes_.size(); }

bool Snake::hitSelf() const { return hitSelf_; }

void Snake::checkSelfCollisions() {
    SnakeNode& headNode = nodes_[0];
    for (decltype(nodes_.size()) i = 1; i < nodes_.size(); ++i) {
        if (headNode.getBounds().intersects(nodes_[i].getBounds())) {
            dieSound_.play();
            sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
            hitSelf_ = true;
        }
    }
}

void Snake::checkEdgeCollisions() {  //穿墙
    SnakeNode& headNode = nodes_[0];
//此处0.2是为了解决浮点数判定问题出现的bug
    if (headNode.getPosition().x < -0.2)
        headNode.setPosition(Game::Width, headNode.getPosition().y);
    else if (headNode.getPosition().x > Game::Width+0.2)
        headNode.setPosition(0, headNode.getPosition().y);
    if (headNode.getPosition().y < -0.2)
        headNode.setPosition(headNode.getPosition().x, Game::Height);
    else if (headNode.getPosition().y > Game::Height+0.2)
        headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move() {
    //添加一个新头
    switch (direction_) {
        case Direction::Up:
            nodes_.push_front(SnakeNode(
                sf::Vector2f(nodes_[0].getPosition().x,
                             nodes_[0].getPosition().y - SnakeNode::Height)));
            break;
        case Direction::Down:
            nodes_.push_front(SnakeNode(
                sf::Vector2f(nodes_[0].getPosition().x,
                             nodes_[0].getPosition().y + SnakeNode::Height)));
            break;
        case Direction::Left:
            nodes_.push_front(SnakeNode(
                sf::Vector2f(nodes_[0].getPosition().x - SnakeNode::Width,
                             nodes_[0].getPosition().y)));
            break;
        case Direction::Right:
            nodes_.push_front(SnakeNode(
                sf::Vector2f(nodes_[0].getPosition().x + SnakeNode::Width,
                             nodes_[0].getPosition().y)));
            break;
    }
}

void Snake::render(sf::RenderWindow& window) {
    for (auto node : nodes_) node.render(window);
}