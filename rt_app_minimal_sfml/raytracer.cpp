#include <iostream>
#include <SFML/Graphics.hpp>

int main (int argc, char* argv[])
{
	std::cout << "Running..." << std::endl;

	sf::Window window(sf::VideoMode(512, 512), "Raytracer");

	while(window.isOpen())
	{
		sf::Event evt;
		while(window.pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
				window.close();
			window.display();
		}
	}

	return 0;
}
