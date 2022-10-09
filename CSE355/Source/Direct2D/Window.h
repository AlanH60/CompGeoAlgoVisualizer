#pragma once
#include "Event.h"
class Graphics;
class Window
{
	public:
		//Maximum events stored the event queue.
		const unsigned int MAX_EVENTS = 64u;
		//The bit of each mouse button in the bitset mMouseState.
		enum MouseButton
		{
			LBUTTON = 0x01,
			RBUTTON = 0x02,
			MBUTTON = 0x03
		};
	public:
		Window(std::string title, int width, int height);
		~Window();
		int getWidth();
		int getHeight();
		std::string getTitle();
		HWND getHandle();
		bool shouldClose();

		//Handles window messages and events from mEventQueue.
		void pollEvents();
		void setOnEvent(const std::function<void(Event&)>& onEvent);

		//Check if a specific mouse button or key is pressed.
		bool lButtonPressed();
		bool rButtonPressed();
		bool mButtonPressed();
		//keycode - is the virtual keycode of a key (ex: VK_LSHIFT).
		bool keyPressed(unsigned char keycode);

		void setGraphics(Graphics* gfx);

		//Handles windows messages by updating mMouseState and mKeyState, and pumping events into mEventQueue.
		LRESULT handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);		
	private:
		void initEventHandle(Event& e);
		void trimQueues();
	private:
		//Pointer to the graphics object used for Direct2D api calls.
		Graphics* pGraphics = nullptr;
		//Dimensions of window
		int mWidth, mHeight;
		//Unique handle for the window.
		HWND mHandle;
		//Flag used to mark that a window close requests has been made.
		bool mShouldClose = false;
		//Event pump from user events(ex: mouse press, key press, etc ...).
		std::queue<std::shared_ptr<Event>> mEventQueue;
		//Handler to handle events from event queue.
		std::function<void(Event&)> onEvent = nullptr;
		//Bitset of all mouse button states.
		std::bitset<3> mMouseState;
		//Bitset of all key states.
		std::bitset<256> mKeyState;
		//Title of the window.
		std::string mTitle;
};

