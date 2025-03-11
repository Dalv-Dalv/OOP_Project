#ifndef SMOOTHINGUTILS_H
#define SMOOTHINGUTILS_H
#include <bits/algorithmfwd.h>

#include "raylib.h"

namespace GameUtilities {
	float lerp(float a, float b, float t);
	float unclampedLerp(float a, float b, float t);

	Vector2 lerp(Vector2 a, Vector2 b, float t);
	Vector2 unclampedLerp(Vector2 a, Vector2 b, float t);
}

#endif //SMOOTHINGUTILS_H
