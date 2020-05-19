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
static int judgepause = 0;
static sf::Mouse mouse;
const int Snake::InitialSize = 5;  //初始长度
Snake::Snake() : hitSelf_(false), dx(0.0), dy(-1.0) {
    initNodes();

    pickupBuffer_.loadFromFile("Sounds/pickup.wav");
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
                                       (SnakeNode::Height * i)),
                      0.0, -1.0));
    }
}

void Snake::handleInput(
    sf::RenderWindow& window) {  //改方向：此处要排除180°转向
    if (!Game::pause) {
        double ndx = dx, ndy = dy;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ndx = 0.0, ndy = -1.0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ndx = 0.0, ndy = 1.0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ndx = -1.0, ndy = 0.0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ndx = 1.0, ndy = 0.0;
        if (mouse.isButtonPressed(sf::Mouse::Left)) {  //读取鼠标状态
            double xx = nodes_[0].getPosition().x,
                   yy = nodes_[0].getPosition().y;
            double ddx = mouse.getPosition(window).x - xx,
                   ddy = mouse.getPosition(window).y - yy;
            if (!(ddx == 0 && ddy == 0))
                ndx = ddx / sqrt((long double)ddx * ddx + ddy * ddy),
                ndy = ddy / sqrt((long double)ddx * ddx + ddy * ddy);
            if ((dx - ndx) * (dx - ndx) + (dy - ndy) * (dy - ndy) < 3.5)
                dx = ndx, dy = ndy;
        }
    }

    if (Game::ingame) {
        judgepause++;
        if (judgepause > 100000000) judgepause -= 50000000;
        if (!Game::pause && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            judgepause > 4) {
            Game::pause = true;
            judgepause = 5000000;
        }

        if (Game::pause && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            judgepause > 5000004) {
            Game::pause = false;
            judgepause = 0;
        }
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
        pile += (*toRemove).sz;
        fruits.erase(toRemove);
    }
    if (!pile)
        nodes_.pop_back();
    else
        pile--;
}

unsigned Snake::getSize() const { return nodes_.size(); }

bool Snake::hitSelf() const { return hitSelf_; }

void Snake::checkSelfCollisions() {
    SnakeNode& headNode = nodes_[0];
    double xx = headNode.getPosition().x, yy = headNode.getPosition().y;
    for (long long unsigned int i = 1; i < nodes_.size(); ++i) {
        double tx = nodes_[i].getPosition().x - xx,
               ty = nodes_[i].getPosition().y - yy;
        // if (headNode.getBounds().intersects(nodes_[i].getBounds()))
        if (tx * tx + ty * ty <= 160) {
            Game::speedup = 0;  //修复加速状态下死后快速开始产生速度过高的bug
            dieSound_.play();
            sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
            hitSelf_ = true;
        }
    }
}

void Snake::checkEdgeCollisions() {  //穿墙
    SnakeNode& headNode = nodes_[0];
    //此处20是为了解决浮点数判定问题出现的bug
    if (headNode.getPosition().x < 0)
        headNode.setPosition(Game::Width - 20, headNode.getPosition().y);
    else if (headNode.getPosition().x > Game::Width - 20)
        headNode.setPosition(0, headNode.getPosition().y);
    if (headNode.getPosition().y < 0)
        headNode.setPosition(headNode.getPosition().x, Game::Height - 20);
    else if (headNode.getPosition().y > Game::Height - 20)
        headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move() {
    //添加一个新头
    nodes_.push_front(SnakeNode(
        sf::Vector2f(nodes_[0].getPosition().x + dx * SnakeNode::Width,
                     nodes_[0].getPosition().y + dy * SnakeNode::Height),
        dx, dy));
}

void Snake::render(sf::RenderWindow& window) {
    for (long long unsigned int i = 0; i < nodes_.size(); ++i)
        nodes_[i].render(window, i);
    if (Game::pause) {
        sf::Font font_;
        sf::Text text_;
        font_.loadFromFile("Fonts/game_over.ttf");  //字体
        text_.setFont(font_);
        text_.setString("Your score: " + std::to_string(Snake::getSize()) +
                        "!"
                        "\n\n\n\nPress [SPACE] to continue");
        text_.setFillColor(sf::Color::Red);
        sf::FloatRect textBounds = text_.getLocalBounds();
        text_.setOrigin(textBounds.left + textBounds.width / 2,
                        textBounds.top + textBounds.height / 2);  //定位
        text_.setPosition(Game::Width / 2, Game::Height / 2);
        window.draw(text_);
    }
}

bool Snake::ok(int x, int y) {
    for (auto node : nodes_) {
        double xx = node.getPosition().x, yy = node.getPosition().y;
        if ((xx - x) * (xx - x) < 480 && (yy - y) * (yy - y) < 480) {
            return false;
        }
    }
    return true;
}