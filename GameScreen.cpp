#include "GameScreen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>

#include "Game.h"
#include "GameOverScreen.h"
#include "SnakeNode.h"

using namespace sfSnake;


GameScreen::GameScreen() : snake_() {
    //生成蛇
}

void GameScreen::handleInput(sf::RenderWindow& window) { snake_.handleInput(); }

void GameScreen::update(sf::Time delta)  //刷新时的处理
{
    if (fruit_.size() == 0) generateFruit();

    snake_.update(delta);
    snake_.checkFruitCollisions(fruit_);

    if (snake_.hitSelf())
        Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize());
    //处理撞墙，传长度，处理分数
}

void GameScreen::render(sf::RenderWindow& window) {
    drawbg(window);
    snake_.render(window);
    for (auto fruit : fruit_) fruit.render(window);
    drawxline(window);
    drawyline(window);
}

void GameScreen::generateFruit() {
    static std::default_random_engine engine(time(NULL));
    static std::uniform_int_distribution<int> xDistribution(
        3*SnakeNode::Width, Game::Width - 3*SnakeNode::Width);
    static std::uniform_int_distribution<int> yDistribution(
        3*SnakeNode::Height, Game::Height - 3*SnakeNode::Height);
    static std::uniform_int_distribution<int> fruitmode(0,7);
    fruit_.push_back(
        Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine)),fruitmode(engine)));
}
void drawbg(sf::RenderWindow& window) {
    auto color = sf::Color::Red;
    switch (Game::bg) {
        case 0:
            color = (sf::Color::White);
            break;
        case 1:
            color = (sf::Color::Black);
            break;
        case 2:
            color = (sf::Color(150, 75, 0));  // brown
            break;
        default:
            color = (sf::Color::White);
    }
    sf::RectangleShape shape_;
    shape_.setPosition(sf::Vector2f(0, 0));
    shape_.setFillColor(color);
    shape_.setSize(sf::Vector2f(Game::Width, Game::Height));
    window.draw(shape_);
}
void drawxline(sf::RenderWindow& window) {
    auto color = sf::Color::Red;
    switch (Game::gridcolor) {
        case 0:
            color = (sf::Color::White);
            break;
        case 1:
            color = (sf::Color::Black);
            break;
        case 2:
            color = (sf::Color(150, 75, 0));  // brown
            break;
        default:
            color = (sf::Color::Transparent);
    }
    for (int i = SnakeNode::Height + 2; i < Game::Height;
         i += SnakeNode::Height + 2) {
        sf::RectangleShape shape_;
        shape_.setPosition(sf::Vector2f(0, i));
        shape_.setFillColor(color);
        shape_.setSize(sf::Vector2f(Game::Width, 1));
        window.draw(shape_);
    }
}
void drawyline(sf::RenderWindow& window) {
    auto color = sf::Color::White;
    switch (Game::gridcolor) {
        case 0:
            color = (sf::Color::White);
            break;
        case 1:
            color = (sf::Color::Black);
            break;
        case 2:
            color = (sf::Color(150, 75, 0));  // brown
            break;
        default:
            color = (sf::Color::Transparent);
    }
    for (int i = SnakeNode::Width + 2; i < Game::Width;
         i += SnakeNode::Width + 2) {
        sf::RectangleShape shape_;
        shape_.setPosition(sf::Vector2f(i, 0));
        shape_.setFillColor(color);
        shape_.setSize(sf::Vector2f(1, Game::Height));
        window.draw(shape_);
    }
}