#pragma once

struct FLOAT2
{
	float x, y;
	float operator[](unsigned int idx);
	FLOAT2 operator+(const FLOAT2& o)
	{
		return { x + o.x, y + o.y };
	}
	FLOAT2 operator-(const FLOAT2& o)
	{
		return { x - o.x, y - o.y };
	}
	FLOAT2 operator*(const int s)
	{
		return { x * s, y * s };
	}
	FLOAT2 operator*(const int s) const
	{
		return { x * s, y * s };
	}
	friend FLOAT2 operator*(const int s, const FLOAT2& f)
	{
		return f * s;
	}
	FLOAT2 operator/(const int s)
	{
		assert(s != 0 && "Division by zero!");
		return { x / s, y / s };
	}
};