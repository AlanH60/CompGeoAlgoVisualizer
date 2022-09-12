#pragma once
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

		LRESULT handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);		
	private:
		std::string mTitle;
		int mWidth, mHeight;
		HWND mHandle;
		bool mShouldClose = false;
};

