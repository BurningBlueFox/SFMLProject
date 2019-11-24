#pragma once
#include "SFML/include/SFML/Graphics.hpp"
#include "Hero.h"

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Hello Test", sf::Style::Default);

sf::Texture skyTexture;
sf::Texture bgTexture;

sf::Sprite skySprite;
sf::Sprite bgSprite;

Hero hero;

sf::Vector2f playerPosition;
bool playerMoving = false;

void init()
{

	// Load sky Texture 
	skyTexture.loadFromFile("Assets/graphics/sky.png");
	bgTexture.loadFromFile("Assets/graphics/bg.png");

	// Set and  Attach Texture to Sprite 
	skySprite.setTexture(skyTexture);
	bgSprite.setTexture(bgTexture);

	hero.init("Assets/graphics/hero.png",
		sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f),
		200);
}

void updateInput()
{

	sf::Event event;

	while (window.pollEvent(event))
	{

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Up)
			{
				hero.jump(750.0f);
			}
		}

		if (event.key.code == sf::Keyboard::Escape || event.type
			== sf::Event::Closed)
			window.close();
	}
}
void update(float dt)
{
	hero.update(dt);
}

void draw()
{
	window.draw(skySprite);
	window.draw(bgSprite);
	window.draw(hero.getSprite());
}

int main()
{
	sf::Clock clock;
	window.setFramerateLimit(60);
	init();
	while (window.isOpen())
	{

		// Update input 
		updateInput();

		// Update Game 
		sf::Time dt = clock.restart();
		update(dt.asSeconds());

		window.clear(sf::Color::Red);

		//Draw Game  
		draw();

		window.display();
	}
	return 0;

}