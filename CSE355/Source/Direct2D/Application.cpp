#include "PCH.h"
#include "Application.h"
#include "Window.h"
#include "Graphics.h"
#include "Drawable/Drawable.h"

Application::Application()
{
	pWindow = new Window("Computational Geometry Visualizer", 1280, 720);
	pGraphics = new Graphics(pWindow);
}

Application::~Application()
{
	for (const auto const* d : drawables)
		delete d;
	delete pGraphics;
	delete pWindow;
}

int Application::run()
{
	while (!pWindow->shouldClose())
	{
		pWindow->pollEvents();
		pGraphics->beginFrame();
		for (int i = 0; i < drawables.size(); i++)
			drawables[i]->draw(*pGraphics);
		pGraphics->endFrame();
	}
	return 0;
}

//Input a drawable pointer allocated by heap
void Application::addDrawable(Drawable* drawable)
{
	drawables.push_back(drawable);
}

void Application::clear()
{
	while(!drawables.empty())
	{
		delete drawables[0];
		drawables.erase(drawables.begin());
	}
}
