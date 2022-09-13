#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Graphics.h"

Application* Application::pInstance;

Application::Application()
{
	pWindow = new Window("Computational Geometry Visualizer", 1280, 720);
	pGraphics = new Graphics(pWindow);
}

Application::~Application()
{
	delete pGraphics;
	delete pWindow;
}

int Application::run()
{
	while (!pWindow->shouldClose())
	{
		pWindow->pollEvents();
		pGraphics->beginFrame();
		pGraphics->endFrame();
	}
	return 0;
}

Application* Application::create()
{
	assert(pInstance == nullptr && "An Instance of application already exists!");
	pInstance = new Application();
	return pInstance;
}

Application* Application::get()
{
	assert(pInstance != nullptr && "Instance of application doesn't exist!");
	return pInstance;
}

void Application::destroy()
{
	delete pInstance;
}
