#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include <rt/scene/scene.hpp>
#include <rt/tracers/simpletracer.hpp>

#include "sfmlbuffer.hpp"

void RenderRegionThread(
	rt::SimpleRayTracer* tracer,
	rt::Scene* scene,
	rt::Buffer* buffer,
	glm::ivec2 renderRegionMin,
	glm::ivec2 renderRegionMax
){
	tracer->Trace(scene, buffer, renderRegionMin, renderRegionMax);
}

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
	glm::ivec2 renderRegions(bufferSize / renderRegionSize);
	renderRegions.x += (bufferSize.x % renderRegionSize.x != 0 ? 1 : 0);
	renderRegions.y += (bufferSize.y % renderRegionSize.y != 0 ? 1 : 0);
	glm::ivec2 currentRenderRegion(0);
	
	sf::RenderWindow window(sf::VideoMode(bufferSize.x, bufferSize.y), "Raytracer");
	rt::SFMLBuffer buffer(bufferSize.x, bufferSize.y);
	rt::SimpleRayTracer tracer;

	const int threadCount = 8;
	std::thread threads[threadCount];
	int threadRenderIndex[threadCount];
	while(window.isOpen() && currentRenderRegion.y <= renderRegions.y)
	{
		for(int t = 0; t < threadCount; ++t)
		{
			threads[t] = std::thread(RenderRegionThread, 
				&tracer, &scene, &buffer, 
				renderRegionSize * currentRenderRegion,
				renderRegionSize * (currentRenderRegion + glm::ivec2(1))
			);

			currentRenderRegion.x++;
			if(currentRenderRegion.x == renderRegions.x)
			{
				currentRenderRegion.x = 0;
				currentRenderRegion.y++;
			}
		}
		for(int t = 0; t < threadCount; ++t)
		{
			threads[t].join();
		}

		buffer.Copy();
		window.draw(buffer.m_SFMLSprite);
		window.display();

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
