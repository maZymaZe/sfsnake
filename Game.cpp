#include "Game.h"

#include <SFML/Graphics.hpp>
#include <memory>

#include "MenuScreen.h"

using namespace sfSnake;

int Game::gridcolor=2;
int Game::bg=0;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 10.f);  //这里大概指10Hz


std::shared_ptr<Screen> Game::Screen = std::make_shared<MenuScreen>();

Game::Game() : window_(sf::VideoMode(Game::Width, Game::Height), "sfSnake") {
    //此处以上可以设置屏幕大小
    bgMusic_.openFromFile("Music/bg_music.wav");
    bgMusic_.setLoop(true);
    bgMusic_.play();
    // bgm的设置，需要放在音乐文件夹并指定
    
}

void Game::handleInput() {
    sf::Event event;

    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window_.close();
    }
    //关闭窗口并退出游戏
    Game::Screen->handleInput(window_);
}

void Game::update(sf::Time delta) { Game::Screen->update(delta); }

void Game::render() {
    window_.clear();
    Game::Screen->render(window_);
    window_.display();
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window_.isOpen()) {
        sf::Time delta = clock.restart();
        timeSinceLastUpdate += delta;
        //处理重启的一小段时间
        while (timeSinceLastUpdate > Game::TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            handleInput();
            update(TimePerFrame);
        }
        //反复刷
        render();
    }
}