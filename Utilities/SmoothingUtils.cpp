#include "SmoothingUtils.h"

#include "Vector2Utils.h"

namespace GameUtilities {
	float lerp(float a, float b, float t) {
		if(t < 0) t = 0;
		if(t > 1) t = 1;
		return a + (b - a) * t;
	}

	float unclampedLerp(float a, float b, float t) {
		return a + (b - a) * t;
	}

	Vector2 lerp(Vector2 a, Vector2 b, float t) {
		if(t < 0) t = 0;
		if(t > 1) t = 1;
		return a + (b - a) * t;
	}

	Vector2 unclampedLerp(Vector2 a, Vector2 b, float t) {
		return a + (b - a) * t;
	}
}