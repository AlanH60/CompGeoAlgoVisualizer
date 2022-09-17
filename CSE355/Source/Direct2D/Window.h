#pragma once
#include "Event.h"
#include <Windows.h>

class Window
{
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

		LRESULT handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);		
	private:
		void initEventHandle(Event& e);
	private:
		std::string mTitle;
		int mWidth, mHeight;
		HWND mHandle;
		bool mShouldClose = false;
		std::queue<std::shared_ptr<Event>> mEventQueue;
		std::function<void(Event&)> onEvent = nullptr;
		enum MouseButton
		{
			LBUTTON = 0x01,
			RBUTTON = 0x02,
			MBUTTON = 0x03
		};
		std::bitset<3> mMouseState;
		std::bitset<256> mKeyState;
};

