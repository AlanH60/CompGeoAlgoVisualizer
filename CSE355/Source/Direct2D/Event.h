#pragma once

struct Event
{
	enum class InputType
	{
		MOUSE = 0x01,
		KEYBOARD = 0x02
	};
	
	enum class EventType
	{
		PRESS = 0x01,
		RELEASE = 0x02,
		MOVE = 0x03
	};

	static bool isMouse(Event& e)
	{
		return e.mInputType == InputType::MOUSE;
	}
	static bool isKeyboard(Event& e)
	{
		return e.mInputType == InputType::KEYBOARD;
	}
	static bool isPress(Event& e)
	{
		return e.mType == EventType::PRESS;
	}
	static bool isRelease(Event& e)
	{
		return e.mType == EventType::RELEASE;
	}

	InputType mInputType;
	EventType mType;
	bool isConsumed = false;
};

struct MouseEvent : public Event
{
	MouseEvent(EventType type, unsigned char keycode, int x, int y) : x(x), y(y), mKeycode(keycode)
	{ 
		mInputType = InputType::MOUSE; 
		mType = type; 
	}
	static bool isMove(Event& e)
	{
		return e.mType == EventType::MOVE;
	}
	int x, y;
	unsigned char mKeycode;
};

struct KeyEvent : public Event
{
	KeyEvent(EventType type, unsigned char keycode) 
		:
		mKeycode(keycode)
	{ 
		assert((int)type <= 0x02 && "Invalid key event type!");
		mInputType = InputType::KEYBOARD;
		mType = type;
	}
	unsigned char mKeycode;
};