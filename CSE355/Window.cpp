#include "PCH.h"
#include "Window.h"


Window::Window(std::string title, int width, int height)
	:
	mTitle(title),
	mWidth(width),
	mHeight(height)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.hInstance = GetModuleHandle(nullptr);
	wchar_t wBuffer[255];
	mbstowcs_s(nullptr, wBuffer, 255, title.c_str(), _TRUNCATE);
	wndClass.lpszClassName = wBuffer;
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = &handleSetup;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = nullptr;
	wndClass.hIconSm = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	RegisterClassEx(&wndClass);

	RECT windowRect = {};
	windowRect.left = 100;
	windowRect.right = width + windowRect.left;
	windowRect.top = 100;
	windowRect.bottom = height + windowRect.top;

	if (!AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, false))
	{
		assert(false && "Failed to create window rect!");
	}
	mHandle = CreateWindowExW(0, wndClass.lpszClassName, wndClass.lpszClassName, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, wndClass.hInstance, this);

	if (mHandle == nullptr)
		assert(false && "Failed to create window handle!");
	ShowWindow(mHandle, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(mHandle);
}

int Window::getWidth()
{
	return mWidth;
}

int Window::getHeight()
{
	return mHeight;
}

std::string Window::getTitle()
{
	return mTitle;
}

HWND Window::getHandle()
{
	return mHandle;
}

bool Window::shouldClose()
{
	return mShouldClose;
}

//Reacts to Window Events. If Window is closed, return false. Otherwise, return true.
void Window::pollEvents()
{
	MSG msg;
	
	while (PeekMessage(&msg, mHandle, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

//Retrieves the Window instance from the creation LParam and puts it into GWLP_USERDATA
LRESULT CALLBACK Window::handleSetup(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* structure = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* window = static_cast<Window*>(structure->lpCreateParams);

		SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&handleAdapter));
		return window->handleMsg(handle, msg, wParam, lParam);
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

//Accesses GWLP_USERDATA that handleSetup() filled with the ptr to Window instance and calls window instance's handleMsg() function.
LRESULT CALLBACK Window::handleAdapter(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	return window->handleMsg(handle, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			mShouldClose = true;
			PostQuitMessage(0);
			return 0;
		default:
			break;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}
