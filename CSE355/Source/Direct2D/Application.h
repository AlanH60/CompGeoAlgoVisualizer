#pragma once

class Window;
class Graphics;
class Application
{
	public:
		Application();
		~Application();

		int run();

		static Application* create();
		static Application* get();
		static void destroy();
	private:
		static Application* pInstance;
		Window* pWindow;
		Graphics* pGraphics;
};

