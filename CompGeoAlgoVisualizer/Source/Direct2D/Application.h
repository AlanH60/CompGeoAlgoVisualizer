#pragma once

class Window;
class Graphics;

/**
* The Application class represents the program itself.  It handles updates and draws and manages the Window and Graphics objects
* to perform such tasks.  Class should be extended to add specialized functionality.
*/
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

