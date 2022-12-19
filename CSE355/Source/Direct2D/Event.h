#pragma once

struct Event
{
	enum class EventType
	{
		PRESS = 0x01,
		RELEASE = 0x02,
		MOVE = 0x03,
		NONE = 0x04
	};

	virtual bool isMouse() { return false; }
	virtual bool isKeyboard() { return false; }
	virtual bool isChar() { return false; }

	bool isPress()
	{
		return mType == EventType::PRESS;
	}
	bool isRelease()
	{
		return mType == EventType::RELEASE;
	}

	EventType mType = EventType::NONE;
	bool isConsumed = false;
};

struct MouseEvent : public Event
{
	MouseEvent(EventType type, unsigned char keycode, int x, int y) : x(x), y(y), mKeycode(keycode)
	{ 
		mType = type; 
	}
	bool isMouse() override { return true; }
	static bool isMove(Event& e)
	{
		return e.mType == EventType::MOVE;
	}
	int x, y;
	unsigned char mKeycode;
};

struct KeyEvent : public Event
{
	enum class SysKeyState : unsigned char
	{
		CTRL = 0x01,
		SHIFT = 0x02,
		ALT = 0x04
	};
	KeyEvent(EventType type, unsigned char keycode, unsigned char keyState)
		:
		mKeycode(keycode),
		mKeyState(keyState)
	{ 
		assert((int)type <= 0x02 && "Invalid key event type!");
		mType = type;
	}
	bool isKeyboard() override { return true; }
	bool ctrlHeld() { return (mKeyState & (unsigned char)SysKeyState::CTRL) == (unsigned char)SysKeyState::CTRL; };
	bool shiftHeld() { return (mKeyState & (unsigned char)SysKeyState::SHIFT) == (unsigned char)SysKeyState::SHIFT; };
	bool altHeld() { return (mKeyState & (unsigned char)SysKeyState::ALT) == (unsigned char)SysKeyState::ALT; };
	unsigned char mKeycode;
	unsigned char mKeyState;
};

struct CharEvent : public Event
{
	CharEvent(wchar_t character)
		:
		mChar(character)
	{}
	bool isChar() override { return true; }

	wchar_t mChar;
};