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



	//const glm::ivec2 bufferSize(960,540);
	const glm::ivec2 bufferSize(640, 360);
	const glm::ivec2 renderRegionSize(8, 8);
	const glm::ivec2 renderRegions(bufferSize / renderRegionSize);
	glm::ivec2 currentRenderRegion(0);
	
	sf::RenderWindow window(sf::VideoMode(bufferSize.x, bufferSize.y), "Raytracer");
	rt::SFMLBuffer buffer(bufferSize.x, bufferSize.y);
	rt::SimpleRayTracer tracer;

	while(window.isOpen() && currentRenderRegion.y <= renderRegions.y)
	{
		tracer.Trace(
			&scene, &buffer, 
			renderRegionSize * currentRenderRegion,
			renderRegionSize * (currentRenderRegion + glm::ivec2(1))
		);
		buffer.Copy();
		window.draw(buffer.m_SFMLSprite);
		window.display();

		currentRenderRegion.x++;
		if(currentRenderRegion.x == renderRegions.x)
		{
			currentRenderRegion.x = 0;
			currentRenderRegion.y++;
		}

		sf::Event evt;
		if(window.pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
				window.close();
		}
	}


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
