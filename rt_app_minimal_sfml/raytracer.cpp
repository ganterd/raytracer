#include <iostream>

#include <SFML/Graphics.hpp>

#include <rt/scene/scene.hpp>
#include <rt/tracers/simpletracer.hpp>
#include "sfmlbuffer.hpp"

int main (int argc, char* argv[])
{
	std::cout << "Running..." << std::endl;

	if(argc < 2)
	{
		std::cerr << "Need file path argument." << std::endl;
		return -1;
	}

	rt::Scene scene;
	scene.fromFile(std::string(argv[1]));

	sf::RenderWindow window(sf::VideoMode(512, 512), "Raytracer");

	rt::SFMLBuffer buffer(512, 512);

	rt::SimpleRayTracer tracer;
	tracer.Trace(&scene, &buffer);
	buffer.Copy();
	window.draw(buffer.m_SFMLSprite);
	window.display();

	while(window.isOpen())
	{
		sf::Event evt;
		while(window.pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
				window.close();
			window.draw(buffer.m_SFMLSprite);
			window.display();
		}
	}

	return 0;
}
