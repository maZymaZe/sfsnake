#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

#include "GameScreen.h"
#include "MenuScreen.h"
#include "Game.h"

using namespace sfSnake;

std::string colorname[4]={" WHITE ", " BLACK "," BROWN "," TRANSPARENT "};

MenuScreen::MenuScreen()
{
	sf::sleep(sf::seconds(1));
	font_.loadFromFile("Fonts/game_over.ttf");
	text_.setFont(font_);
	text_.setString(
		"\n\n\n\n\n\n\n\n\nPress [SPACE] to play"
		"\n\nPress [ESC] to quit");
	text_.setCharacterSize(24);
	snakeText_.setFont(font_);
	snakeText_.setString("Snake!");
	snakeText_.setColor(sf::Color::Green);
	snakeText_.setCharacterSize(48);
	snakeText_.setStyle(sf::Text::Bold);

	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2,
		textBounds.top + textBounds.height / 2);
	text_.setPosition(Game::Width / 2, Game::Height / 2);

	sf::FloatRect snakeTextBounds = snakeText_.getLocalBounds();
	snakeText_.setOrigin(snakeTextBounds.left + snakeTextBounds.width / 2,
		snakeTextBounds.top + snakeTextBounds.height / 2);
	snakeText_.setPosition(Game::Width / 2, Game::Height / 4);

	//以上位置、内容的处理
}

void MenuScreen::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Game::Screen = std::make_shared<GameScreen>();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window.close();
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		Game::bg=(Game::bg+1)%3;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		Game::gridcolor=(Game::gridcolor+1)%4;
}

void MenuScreen::update(sf::Time delta)
{
	static bool movingLeft = false;
	static bool movingRight = true;
//旋转效果
	if (movingRight)
	{
		snakeText_.rotate(delta.asSeconds());
	
		if (static_cast<int>(snakeText_.getRotation()) == 10)
		{
			movingRight = false;
			movingLeft = true;
		}
	}

	if (movingLeft)
	{
		snakeText_.rotate(-delta.asSeconds());

		if (static_cast<int>(snakeText_.getRotation()) == (360 - 10))
		{
			movingLeft = false;
			movingRight = true;
		}
	}
	std::string texttmp="\n\n\n\nPress [SPACE] to play\n\nPress [ESC] to quit"
	"\n\nPress [B] to switch\n\nbackground color->"+colorname[Game::bg]+
	"\n\nPress [G] to switch\n\ngrid color->"+colorname[Game::gridcolor];
	text_.setString(texttmp);
}

void MenuScreen::render(sf::RenderWindow& window)
{
	window.draw(text_);
	window.draw(snakeText_);
}