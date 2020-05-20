#include "GameScreen.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>

#include "Game.h"
#include "GameOverScreen.h"
#include "SnakeNode.h"

using namespace sfSnake;

sf::Color colorname[4] = {(sf::Color::White), (sf::Color::Black),
                          (sf::Color(150, 75, 0)), (sf::Color::Transparent)};

GameScreen::GameScreen() : snake_() {
    //生成蛇
    Game::ingame = true;
    Game::pause = false;
}

void GameScreen::handleInput(sf::RenderWindow& window) {
    snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)  //刷新时的处理
{
    while (fruit_.size() < 3) generateFruit();

    snake_.update(delta);
    snake_.checkFruitCollisions(fruit_);

    if (snake_.getSize() >= 4000 || snake_.hitSelf())
        Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize()),
        Game::ingame = false;
    //处理撞墙，传长度，处理分数
}

void GameScreen::render(sf::RenderWindow& window) {
    drawbg(window);
    drawxline(window);
    drawyline(window);
    snake_.render(window);
    for (auto fruit : fruit_) fruit.render(window);
}

void GameScreen::generateFruit() {
    static std::default_random_engine engine(time(NULL));
    static std::uniform_int_distribution<int> xDistribution(
        3 * SnakeNode::Width, Game::Width - 3 * SnakeNode::Width);
    static std::uniform_int_distribution<int> yDistribution(
        3 * SnakeNode::Height, Game::Height - 3 * SnakeNode::Height);
    static std::uniform_int_distribution<int> fruitmode(0, 7);
    int tx = xDistribution(engine), ty = yDistribution(engine);
    while (!snake_.ok(tx, ty)||(!GameScreen::foodok(tx,ty))) {  //检测是否合法生成位置
        tx = xDistribution(engine), ty = yDistribution(engine);
    }
    fruit_.push_back(Fruit(sf::Vector2f(tx, ty), fruitmode(engine)));
}
void drawbg(sf::RenderWindow& window) {
    auto color = colorname[Screen::bg];
    sf::RectangleShape shape_;
    shape_.setPosition(sf::Vector2f(0, 0));
    shape_.setFillColor(color);
    shape_.setSize(sf::Vector2f(Game::Width, Game::Height));
    window.draw(shape_);
}
void drawxline(sf::RenderWindow& window) {
    auto color = colorname[Screen::gridcolor];
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
    auto color = colorname[Screen::gridcolor];
    for (int i = SnakeNode::Width + 2; i < Game::Width;
         i += SnakeNode::Width + 2) {
        sf::RectangleShape shape_;
        shape_.setPosition(sf::Vector2f(i, 0));
        shape_.setFillColor(color);
        shape_.setSize(sf::Vector2f(1, Game::Height));
        window.draw(shape_);
    }
}
bool GameScreen::foodok(int x, int y) {
    for (auto node : fruit_) {
        double xx = node.xposition, yy = node.yposition;
        if ((xx - x) * (xx - x) + (yy - y) * (yy - y) < 6000) {
            return false;
        }
    }
    return true;
}