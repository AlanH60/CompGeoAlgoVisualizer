#pragma once

class Window;
class Graphics;
class Drawable;
class Application
{
	public:
		Application();
		~Application();

		int run();
		void addDrawable(Drawable* drawable);
		void clear();
	protected:
		Window* pWindow;
		Graphics* pGraphics;
		std::vector<Drawable*> drawables;
};

