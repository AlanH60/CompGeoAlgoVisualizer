#pragma once
struct Vector2f;

class Loader
{
	public:
		static bool loadIPE(std::vector<Vector2f>& v, std::string file);
};

