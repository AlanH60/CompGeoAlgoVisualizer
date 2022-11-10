#pragma once

class Window;
class Graphics;
class Application
{
	public:
		Application();
		virtual ~Application();

		int run();
		virtual void onUpdate() {};
		virtual void onDraw() {};
	protected:
		Window* pWindow;
		Graphics* pGraphics;
};

