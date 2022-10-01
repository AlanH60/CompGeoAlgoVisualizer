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
	delete pGraphics;
	delete pWindow;
}

int Application::run()
{
	while (!pWindow->shouldClose())
	{
		pWindow->pollEvents();
		pGraphics->beginFrame();
		onDraw();
		pGraphics->endFrame();
	}
	return 0;
}