#pragma once

/**
* Events are initialized by and contain information about user input. 
*/
struct Event
{
	virtual bool isMouse() { return false; }
	virtual bool isKeyboard() { return false; }
	virtual bool isChar() { return false; }
	virtual bool isWindow() { return false; }
	bool isConsumed = false;
};

struct MouseEvent : public Event
{
	enum class Type
	{
		PRESS = 0x01,
		RELEASE,
		MOVE
	};
	MouseEvent(Type type, unsigned char keycode, int x, int y) : x(x), y(y), mKeycode(keycode), mType(type) {}
	bool isMouse() override { return true; }
	bool isPress() { return mType == Type::PRESS; }
	bool isRelease() { return mType == Type::RELEASE; }
	bool isMove() { return mType == Type::MOVE; }

	int x, y;
	Type mType;
	unsigned char mKeycode;
};

struct KeyEvent : public Event
{
	enum class Type
	{
		PRESS = 0x01,
		RELEASE
	};
	enum class SysKeyState : unsigned char
	{
		CTRL = 0x01,
		SHIFT = 0x02,
		ALT = 0x04
	};
	KeyEvent(Type type, unsigned char keycode, unsigned char keyState) : mKeycode(keycode),	mKeyState(keyState), mType(type) {}
	bool isKeyboard() override { return true; }
	bool ctrlHeld() { return (mKeyState & (unsigned char)SysKeyState::CTRL); };
	bool shiftHeld() { return (mKeyState & (unsigned char)SysKeyState::SHIFT); };
	bool altHeld() { return (mKeyState & (unsigned char)SysKeyState::ALT); };
	bool isPress() { return mType == Type::PRESS; }
	bool isRelease() { return mType == Type::RELEASE; }

	Type mType;
	unsigned char mKeycode;
	unsigned char mKeyState;
};

struct CharEvent : public Event
{
	CharEvent(wchar_t character) : mChar(character)	{}
	bool isChar() override { return true; }

	wchar_t mChar;
};

struct WindowEvent : public Event
{
	enum class Type
	{
		RESIZE = 0x01
	};
	WindowEvent(Type type, int windowWidth, int windowHeight) : mType(type), mWindowWidth(windowWidth), mWindowHeight(windowHeight) {}
	bool isWindow() override { return true; }
	bool isResize() { return mType == Type::RESIZE; }

	Type mType;
	int mWindowWidth, mWindowHeight;
};