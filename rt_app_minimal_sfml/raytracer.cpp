#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include <rt/scene/scene.hpp>
#include <rt/tracers/simpletracer.hpp>
#include <rt/tracers/bvhtracer.hpp>
#include <rt/utils/timer.hpp>

#include "sfmlbuffer.hpp"

void RenderRegionThread(
	rt::RayTracer* tracer,
	rt::Scene* scene,
	rt::Buffer* buffer,
	glm::ivec2 renderRegionMin,
	glm::ivec2 renderRegionMax
){
	tracer->Trace(scene, buffer, renderRegionMin, renderRegionMax);
}

int main (int argc, char* argv[])
{
	rt::Scene scene;
	scene.fromFile(std::string(argv[1]));

	rt::BVHRayTracer tracer;

	int threadCount = 8;
	glm::ivec2 bufferSize(960, 540);
	glm::ivec2 renderRegionSize(16, 16);

	for(int i = 2; i < argc; ++i)
	{
		std::string arg(argv[i]);
		std::string param = i + 1 < argc ? argv[i + 1] : "";		
		if(arg == "-b")
			tracer.mSettings.bounces = std::stoi(param);
		else if(arg == "-bs")
			tracer.mSettings.bounceSamples = std::stoi(param);
		else if(arg == "-t")
			threadCount = std::stoi(param);
		else if(arg == "-bsx")
			bufferSize.x = std::stoi(param);
		else if(arg == "-bsy")
			bufferSize.y = std::stoi(param);
		else if(arg == "-rsx")
			renderRegionSize.x = std::stoi(param);
		else if(arg == "-rsy")
			renderRegionSize.y = std::stoi(param);
	}

	//const glm::ivec2 bufferSize(640, 360);
	
	glm::ivec2 renderRegions(bufferSize / renderRegionSize);
	renderRegions.x += (bufferSize.x % renderRegionSize.x != 0 ? 1 : 0);
	renderRegions.y += (bufferSize.y % renderRegionSize.y != 0 ? 1 : 0);
	glm::ivec2 currentRenderRegion(0);
	
	sf::RenderWindow window(sf::VideoMode(bufferSize.x, bufferSize.y), "Raytracer");
	rt::SFMLBuffer buffer(bufferSize.x, bufferSize.y);
	
	//rt::SimpleRayTracer tracer;
	tracer.init(&scene);

	
	std::thread* threads = new std::thread[threadCount];
	int* threadRenderIndex = new int[threadCount];

	std::cout << "Tracing..." << std::endl;
	float actualRenderTime = 0.0f;
	
	
	rt::Timer actualRenderTimer, renderAndCopyTimer;
	renderAndCopyTimer.start();
	while(window.isOpen() && currentRenderRegion.y <= renderRegions.y)
	{
		actualRenderTimer.start();
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
		actualRenderTime += actualRenderTimer.stop();

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
	renderAndCopyTimer.stop();
	delete[] threads;
	delete[] threadRenderIndex;
	std::cout << "Done in " << renderAndCopyTimer.getTime() << "s (render " << actualRenderTime << "s)" << std::endl;

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
