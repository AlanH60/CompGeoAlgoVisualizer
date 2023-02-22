#pragma once
#include "Event.h"

/**
* Window class represents a window created using WINAPI.
*/
class Window
{
	friend class Application;
	friend class Graphics;
	public:
		//Maximum events stored the event queue.
		static const unsigned int MAX_EVENTS = 64u;
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

		/**
		* Sets the function that will handle events from mEventQueue.
		* @param onEvent reference to the event handler function.
		*/
		void setOnEvent(const std::function<void(Event&)>& onEvent);

		/**
		* Check if the left mouse button is pressed.
		* @returns true if button is pressed.
		*/
		bool lButtonPressed();
		/**
		* Check if the right mouse button is pressed.
		* @returns true if button is pressed.
		*/
		bool rButtonPressed();
		/**
		* Check if the middle mouse button is pressed.
		* @returns true if button is pressed.
		*/
		bool mButtonPressed();
		/**
		* Check if a specific key is pressed 
		* @param keycode the virtual keycode of a key (ex: VK_LSHIFT).
		* @returns true if key is pressed.
		*/
		bool keyPressed(unsigned char keycode);

	private:
		/************************Used by Application class**************************/
		//Handles window messages and events from mEventQueue.
		void pollEvents();
		//Determines when the program should end because window is closed.
		bool shouldClose();
		/***************************************************************************/

		//Links the windows and graphics api(Used by Graphics class)
		void setGraphics(Graphics* gfx);

		//Window msg handler
		LRESULT handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);

		//Updates mouse and key states when necessary.
		void initEventHandle(Event& e);
		//Pops queue if event queue exceeds MAX_EVENTS
		void trimQueue();
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

