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
	for (const auto const* d : mDrawables)
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
		for (Drawable* d: mDrawables)
			d->draw(*pGraphics);
		onDraw();
		pGraphics->endFrame();
	}
	return 0;
}

//Input a drawable pointer allocated by heap
void Application::addDrawable(Drawable* drawable)
{
	mDrawables.insert(drawable);
}

void Application::clear()
{
	for (Drawable* d : mDrawables)
	{
		delete d;
	}
	mDrawables.clear();
}
