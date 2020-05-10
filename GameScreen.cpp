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
    snake_.render(window);
    for (auto fruit : fruit_) fruit.render(window); 
    if (Game::drawgrid) {
        drawxline(window);
        drawyline(window);
    }
}

void GameScreen::generateFruit() {
    static std::default_random_engine engine(time(NULL));
    static std::uniform_int_distribution<int> xDistribution(
        10, Game::Width - SnakeNode::Width);
    static std::uniform_int_distribution<int> yDistribution(
        10, Game::Height - SnakeNode::Height);

    fruit_.push_back(
        Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine))));
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
            color = (sf::Color::White);
    }
    sf::VertexArray line(sf::Lines, Game::Height / SnakeNode::Height + 1);
    for (int i = 0; i <= Game::Height; i += SnakeNode::Height) {
        line[i / SnakeNode::Height] =
            sf::Vertex(sf::Vector2f(0, i), color, sf::Vector2f(Game::Width, i));
    }
    window.draw(line);
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
            color = (sf::Color::White);
    }
    sf::VertexArray line(sf::Lines, Game::Width / SnakeNode::Width + 1);
    for (int i = 0; i <= Game::Width; i += SnakeNode::Width) {
        line[i / SnakeNode::Width] = sf::Vertex(sf::Vector2f(0, i), color,
                                                sf::Vector2f(Game::Height, i));
    }
    window.draw(line);
}