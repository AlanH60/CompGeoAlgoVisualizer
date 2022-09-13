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
		RELEASE = 0x02
	};

	InputType mInputType;
	EventType mType;
	unsigned char mKeycode;
};

struct MouseEvent : public Event
{
	MouseEvent(EventType type, unsigned char keycode, int x, int y) : x(x), y(y) { mInputType = InputType::MOUSE; mType = type; mKeycode = keycode; }
	int x, y;
};

struct KeyEvent : public Event
{
	KeyEvent(EventType type, unsigned char keycode) { mInputType = InputType::KEYBOARD; mType = type; mKeycode = keycode; }
};