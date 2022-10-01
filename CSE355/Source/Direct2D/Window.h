#pragma once
#include "Event.h"
class Graphics;
class Window
{
	public:
		const unsigned int MAX_EVENTS = 64u;
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

		void pollEvents();
		void setOnEvent(const std::function<void(Event&)>& onEvent);

		bool lButtonPressed();
		bool rButtonPressed();
		bool mButtonPressed();
		bool keyPressed(unsigned char keycode);

		void setGraphics(Graphics* gfx);

		LRESULT handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);		
	private:
		void initEventHandle(Event& e);
		void trimQueues();
	private:
		Graphics* pGraphics = nullptr;
		int mWidth, mHeight;
		HWND mHandle;
		bool mShouldClose = false;
		std::queue<std::shared_ptr<Event>> mEventQueue;
		std::function<void(Event&)> onEvent = nullptr;
		std::bitset<3> mMouseState;
		std::bitset<256> mKeyState;
		std::string mTitle;
};

