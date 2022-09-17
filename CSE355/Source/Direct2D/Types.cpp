#include "PCH.h"
#include "Types.h"

float FLOAT2::operator[](unsigned int idx)
{
	return *(reinterpret_cast<float*>(this) + idx);
}