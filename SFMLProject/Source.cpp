#pragma once
#include "SFML/include/SFML/Graphics.hpp"
#include "SFML/include/SFML/Audio.hpp"
#include <vector>
#include "Enemy.h"
#include "Hero.h"
#include "Rocket.h"

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Hello Test", sf::Style::Default);

void spawnEnemy();
void shoot();

bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2);
void reset();

sf::Texture skyTexture;
sf::Texture bgTexture;

sf::Sprite skySprite;
sf::Sprite bgSprite;

sf::Music bgMusic;

sf::SoundBuffer fireBuffer;
sf::SoundBuffer hitBuffer;

sf::Sound fireSound(fireBuffer);
sf::Sound hitSound(hitBuffer);

Hero hero;
std::vector<Enemy*> enemies;
std::vector<Rocket*> rockets;

float currentTime;
float prevTime = 0.0f;

int score = 0;
bool isGameover = true;

//Text
sf::Font headingFont;
sf::Text headingText;

sf::Font scoringFont;
sf::Text scoringText;

sf::Text tutorialText;

sf::Vector2f playerPosition;
bool playerMoving = false;

void init()
{
	//Audio
	bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	bgMusic.setVolume(40.0f);
	bgMusic.setLoop(true);
	bgMusic.play();

	hitBuffer.loadFromFile("Assets/audio/hit.ogg");
	fireBuffer.loadFromFile("Assets/audio/fire.ogg");

	// Load sky Texture 
	skyTexture.loadFromFile("Assets/graphics/sky.png");
	bgTexture.loadFromFile("Assets/graphics/bg.png");

	// Set and  Attach Texture to Sprite 
	skySprite.setTexture(skyTexture);
	bgSprite.setTexture(bgTexture);

	headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");
	headingText.setFont(headingFont);
	headingText.setString("Tiny Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Red);

	scoringFont.loadFromFile("Assets/fonts/arial.ttf");
	scoringText.setFont(scoringFont);
	scoringText.setString("Score:0");
	scoringText.setCharacterSize(45);
	scoringText.setFillColor(sf::Color::Red);

	// Tutorial Text 

	tutorialText.setFont(scoringFont);
	tutorialText.setString("Press Down Arrow to Fire and Start Game, Up Arrow to Jump");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Red);


	sf::FloatRect headingBounds = headingText.getLocalBounds();
	headingText.setOrigin(headingBounds.width / 2, headingBounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.1f));

	sf::FloatRect scoringBounds = scoringText.getLocalBounds();
	scoringText.setOrigin(scoringBounds.width / 2, scoringBounds.height / 2);
	scoringText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.1f));
	sf::FloatRect tutorialbounds = tutorialText.getLocalBounds();

	tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));

	hero.init("Assets/graphics/heroAnim.png", 4, 1.0f,
		sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f),
		200);

	srand((int)time(0));
}

void spawnEnemy()
{

	int randLoc = rand() % 3;

	sf::Vector2f enemyPos;

	float speed;

	switch (randLoc)
	{

	case 0: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f);
		speed = -400; break;

	case 1: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f);
		speed = -550; break;

	case 2: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f);
		speed = -650; break;

	default: printf("incorrect y value \n"); return;

	}

	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos, speed);

	enemies.push_back(enemy);
}

void shoot()
{
	Rocket* rocket = new Rocket();
	rocket->init("Assets/graphics/rocket.png", hero.getSprite().getPosition(), 400.0f);

	rockets.push_back(rocket);
	fireSound.play();
}

bool checkCollision(sf::Sprite sprite1, sf::Sprite sprite2)
{
	sf::FloatRect shape1 = sprite1.getGlobalBounds();
	sf::FloatRect shape2 = sprite2.getGlobalBounds();

	if (shape1.intersects(shape2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void reset()
{
	score = 0;
	currentTime = 0.0f;
	prevTime = 0.0f;
	scoringText.setString("Score:0");

	for (Enemy* enemy : enemies)
	{
		delete(enemy);
	}

	for (Rocket* rocket : rockets)
	{
		delete(rocket);
	}

	enemies.clear();
	rockets.clear();
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
			if (event.key.code == sf::Keyboard::Down)
			{
				if (isGameover)
				{
					isGameover = false;
					reset();
				}
				else
				{
					shoot();
				}

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

	currentTime += dt;
	//SpawnEnemies
	if (currentTime >= prevTime + 1.125f)
	{
		spawnEnemy();
		prevTime = currentTime;
	}

	// Update Enemies 

	for (int i = 0; i < enemies.size(); i++)
	{

		Enemy* enemy = enemies[i];

		enemy->update(dt);

		if (enemy->getSprite().getPosition().x < 0)
		{

			enemies.erase(enemies.begin() + i);
			delete(enemy);
			isGameover = true;

		}
	}

	//Update Rockets

	for (int i = 0; i < rockets.size(); i++)
	{
		Rocket* rocket = rockets[i];

		rocket->update(dt);

		if (rocket->getSprite().getPosition().x > viewSize.x)
		{
			rockets.erase(rockets.begin() + i);
			delete(rocket);
		}
	}

	//Check collision between rocket and enemie
	for (int i = 0; i < rockets.size(); i++)
	{
		for (int j = 0; j < enemies.size(); j++)
		{

			Rocket* rocket = rockets[i];
			Enemy* enemy = enemies[j];

			if (checkCollision(rocket->getSprite(), enemy->getSprite()))
			{
				score++;
				hitSound.play();

				std::string finalScore = "Score:" + std::to_string(score);
				scoringText.setString(finalScore);
				sf::FloatRect scoringBounds = scoringText.getLocalBounds();
				scoringText.setOrigin(scoringBounds.width / 2, scoringBounds.height / 2);
				scoringText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.1f));

				rockets.erase(rockets.begin() + i);
				enemies.erase(enemies.begin() + j);

				delete(rocket);
				delete(enemy);

				printf("Rocket intersects enemy \n");
			}

		}
	}

}

void draw()
{
	window.draw(skySprite);
	window.draw(bgSprite);
	window.draw(hero.getSprite());

	for (Enemy* enemy : enemies)
	{
		window.draw(enemy->getSprite());
	}

	for (Rocket* rocket : rockets)
	{
		window.draw(rocket->getSprite());
	}

	if (isGameover)
	{
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else
	{
		window.draw(scoringText);
	}
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

		if (!isGameover)
			update(dt.asSeconds());

		window.clear(sf::Color::Red);

		//Draw Game  
		draw();

		window.display();
	}
	return 0;

}