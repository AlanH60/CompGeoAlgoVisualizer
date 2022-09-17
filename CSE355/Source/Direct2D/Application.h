#pragma once

class Window;
class Graphics;
class Drawable;
class Application
{
	public:
		Application();
		virtual ~Application();

		int run();
		virtual void onDraw() {};
		void addDrawable(Drawable* drawable);
		void clear();
	protected:
		Window* pWindow;
		Graphics* pGraphics;
		std::set<Drawable*> mDrawables;
};

