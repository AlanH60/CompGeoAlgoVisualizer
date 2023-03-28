#include "PCH.h"
#include "Vector2f.h"
#include "Vector2D.h"


Vector2f::operator Vector2D()
{
	return Vector2D((double)x, (double)y);
}
