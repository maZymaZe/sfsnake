#include "Snake.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <memory>

#include "Fruit.h"
#include "Game.h"
#include "GameOverScreen.h"
#include "SnakeNode.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;  //初始长度
Snake::Snake() : dx(0), dy(-1.0), hitSelf_(false) {
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
        dx = 0, dy = -1.0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        dx = 0, dy = 1.0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        dx = -1.0, dy = 0;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        dx = 1.0, dy = 0;

    if (Game::getmouse) {
        double xx = nodes_[0].getPosition().x, yy = nodes_[0].getPosition().y;
        double ddx = Game::mousex - xx, ddy = Game::mousey - yy;
        dx = ddx / sqrt((long double)ddx * ddx + ddy * ddy);
        dy = ddy / sqrt((long double)ddx * ddx + ddy * ddy);
    }
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
        if ((*toRemove).sz == 0)
            nodes_.pop_back();
        else if ((*toRemove).sz == 1)
            ;
        else if ((*toRemove).sz == 2) {
            move();
            checkEdgeCollisions();
            checkSelfCollisions();
        }
        else if ((*toRemove).sz == 3) {
            move();
            checkEdgeCollisions();
            checkSelfCollisions();
            move();
            checkEdgeCollisions();
            checkSelfCollisions();
        }
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
    double xx = headNode.getPosition().x, yy = headNode.getPosition().y;
    for (decltype(nodes_.size()) i = 1; i < nodes_.size(); ++i) {
        double tx = nodes_[i].getPosition().x - xx,
               ty = nodes_[i].getPosition().y - yy;
        // if (headNode.getBounds().intersects(nodes_[i].getBounds()))
        if (tx * tx + ty * ty <= 40) {
            dieSound_.play();
            sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
            hitSelf_ = true;
        }
    }
}

void Snake::checkEdgeCollisions() {  //穿墙
    SnakeNode& headNode = nodes_[0];
    //此处0.01是为了解决浮点数判定问题出现的bug
    if (headNode.getPosition().x < 0)
        headNode.setPosition(Game::Width - 10, headNode.getPosition().y);
    else if (headNode.getPosition().x > Game::Width - 10)
        headNode.setPosition(0, headNode.getPosition().y);
    if (headNode.getPosition().y < 0)
        headNode.setPosition(headNode.getPosition().x, Game::Height - 10);
    else if (headNode.getPosition().y > Game::Height - 10)
        headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move() {
    //添加一个新头
    nodes_.push_front(SnakeNode(
        sf::Vector2f(nodes_[0].getPosition().x + dx * SnakeNode::Width,
                     nodes_[0].getPosition().y + dy * SnakeNode::Height)));
}

void Snake::render(sf::RenderWindow& window) {
    for (auto node : nodes_) node.render(window);
}