#include "PCH.h"
#include "Loader.h"
#include "Primatives.h"

bool Loader::loadIPE(std::vector<Vector2f>& v, std::string file)
{
	std::ifstream ifs = std::ifstream(file);
	if (ifs.fail()) //Can't read file
		return false;
	std::string buffer;
	while (std::getline(ifs, buffer))
	{
		//Relies on the fact that the polygon is drawn using black heavier pen
		if (buffer == "<path layer=\"alpha\" stroke=\"black\" pen=\"heavier\">")
			break;
	}
	std::getline(ifs, buffer);
	if (ifs.eof()) //Can't find points of polygon
		return false;
	while (std::getline(ifs, buffer))
	{
		if (buffer == "</path>")
			break;
		int space = buffer.find_first_of(' ', 0);
		int lSpace = buffer.find_last_of(' ');
		std::string coordinates[2] = { buffer.substr(0, space), buffer.substr(space + 1, lSpace - space - 1) };
		v.emplace_back(Vector2f{ std::stof(coordinates[0].c_str()), std::stof(coordinates[1].c_str()) });
	}
	return true;
}