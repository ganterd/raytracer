#include <iostream>
#include <thread>
#include <mutex>

#include <SFML/Graphics.hpp>

#include <rt/scene/scene.hpp>
#include <rt/tracers/simpletracer.hpp>
#include <rt/tracers/bvhtracer.hpp>
#include <rt/utils/timer.hpp>

#include "sfmlbuffer.hpp"

int threadCount;
glm::ivec2 renderRegions;
int totalRenderRegions;
glm::ivec2 renderRegionSize;


glm::ivec4* currentThreadRenderRegions;

std::mutex remainingRenderRegionsMutex;
int totalRemainingRenderRegions;

void RenderRegionThread(
	rt::RayTracer* tracer,
	rt::Scene* scene,
	rt::Buffer* buffer,
	int threadId
){
	int currentRenderRegionIndex = threadId;
	while(currentRenderRegionIndex < totalRenderRegions)
	{
		glm::ivec2 renderRegion;
		renderRegion.x = currentRenderRegionIndex % renderRegions.x;
		renderRegion.y = currentRenderRegionIndex / renderRegions.x;

		glm::ivec2 renderRegionMin = renderRegion * renderRegionSize;
		glm::ivec2 renderRegionMax = renderRegionMin + renderRegionSize; 

		remainingRenderRegionsMutex.lock();
		currentThreadRenderRegions[threadId].x = renderRegionMin.x;
		currentThreadRenderRegions[threadId].y = renderRegionMin.y;
		currentThreadRenderRegions[threadId].z = renderRegionMax.x;
		currentThreadRenderRegions[threadId].w = renderRegionMax.y;
		remainingRenderRegionsMutex.unlock();
		
		tracer->Trace(scene, buffer, renderRegionMin, renderRegionMax, threadId);

		remainingRenderRegionsMutex.lock();
		currentRenderRegionIndex += threadCount;
		totalRemainingRenderRegions--;
		remainingRenderRegionsMutex.unlock();
	}
}

int main (int argc, char* argv[])
{
	rt::Scene scene;
	scene.fromFile(std::string(argv[1]));

	rt::BVHRayTracer tracer;

	threadCount = 8;
	glm::ivec2 bufferSize(960, 540);
	renderRegionSize = glm::vec2(16, 16);

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
	
	renderRegions = glm::ivec2(bufferSize / renderRegionSize);
	renderRegions.x += (bufferSize.x % renderRegionSize.x != 0 ? 1 : 0);
	renderRegions.y += (bufferSize.y % renderRegionSize.y != 0 ? 1 : 0);
	totalRenderRegions = renderRegions.x * renderRegions.y;
	totalRemainingRenderRegions = totalRenderRegions;
	
	sf::RenderWindow window(sf::VideoMode(bufferSize.x, bufferSize.y), "Raytracer");
	rt::SFMLBuffer buffer(bufferSize.x, bufferSize.y, threadCount);
	
	//rt::SimpleRayTracer tracer;
	tracer.init(&scene);

	
	std::thread* threads = new std::thread[threadCount];
	int* threadRenderIndex = new int[threadCount];

	std::cout << "Tracing..." << std::endl;
	float actualRenderTime = 0.0f;
	
	
	rt::Timer actualRenderTimer, renderAndCopyTimer;
	renderAndCopyTimer.start();
	
	currentThreadRenderRegions = new glm::ivec4[threadCount];
	actualRenderTimer.start();
	int remaining = totalRemainingRenderRegions;
	for(int t = 0; t < threadCount; ++t)
	{
		threads[t] = std::thread(RenderRegionThread, &tracer, &scene, &buffer, t);
	}
	
	while(remaining)
	{
		window.clear();
		buffer.Copy();
		window.draw(buffer.m_SFMLSprite);

		remainingRenderRegionsMutex.lock();
		remaining = totalRemainingRenderRegions;
		for(int i = 0; i < threadCount; ++i)
		{
			sf::RectangleShape renderRegion(sf::Vector2<float>(renderRegionSize.x, renderRegionSize.y));
			renderRegion.setPosition(currentThreadRenderRegions[i].x, bufferSize.y - currentThreadRenderRegions[i].y - renderRegionSize.y);
			renderRegion.setFillColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
			renderRegion.setOutlineColor(sf::Color(255.0f, 0.0, 0.0f, 255.0f));
			renderRegion.setOutlineThickness(-1.0f);
			window.draw(renderRegion);
		}
		remainingRenderRegionsMutex.unlock();

		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		sf::Event evt;
		if(window.pollEvent(evt))
		{
			if(evt.type == sf::Event::Closed)
				window.close();
		}
	}

	// Join all threads
	for(int t = 0; t < threadCount; ++t)
	{
		threads[t].join();
	}

	// Draw the final image
	buffer.Copy();
	window.draw(buffer.m_SFMLSprite);
	window.display();

	

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
