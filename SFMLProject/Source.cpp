#include "SFML/include/SFML/Graphics.hpp"

sf::Vector2f viewSize(1024, 768);
sf::VideoMode vm(viewSize.x, viewSize.y);
sf::RenderWindow window(vm, "Hello Test", sf::Style::Default);

int main()
{
	//Init GameObjects
	while (window.isOpen())
	{
		//Handle keyboard events
		//Updates GameObjects on the scene


		window.clear(sf::Color::Cyan);
		//Render GameObjects
		window.display();
	}

	return 0;
}