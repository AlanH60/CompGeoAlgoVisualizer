#include "PCH.h"
#include "Algorithms.h"
#include "Primatives.h"
#include "Loader.h"

#include "App.h"

#include <Windows.h>


int main(int argc, char** argv)
{
	std::vector<Vector2f> vertices;
	if (!Loader::loadIPE(vertices, "C:/Users/Hau/Downloads/hw1-polygon-count-tri.ipe"))
		return 1;
	for (int i = 0; i < vertices.size(); i++)
	{
		Vector2f& b = vertices[(i - 1) % vertices.size()];
		Vector2f& c = vertices[i];
		Vector2f& a = vertices[(i + 1) % vertices.size()];
		std::cout << (i + 5) % vertices.size() << " (" << c.x << ", " << c.y << ") " << ((isConvex(b, c, a)) ? " Convex" : " Concave") << std::endl;
	}

	
	auto c = convexHullGW(vertices);
	for (const auto& v : c)
	{
		std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
	}
	std::cout << "Graham" << std::endl;
	c = convexHullGraham(vertices);
	for (const auto& v : c)
	{
		std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
	}
	std::cout << "Quick Hull" << std::endl;
	c = quickHull(vertices);
	for (const auto& v : c)
	{
		std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
	}
	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOW);
}

int CALLBACK WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow
)
{
	App* app = new App();

	int ret = app->run();

	delete app;
	return ret;
}
