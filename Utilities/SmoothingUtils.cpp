#include "SmoothingUtils.h"

#include "Vector2Utils.h"

namespace GameUtilities {
	float lerp(float a, float b, float t) {
		t = std::clamp(t, 0.0f, 1.0f);
		return a + (b - a) * t;
	}

	float unclampedLerp(float a, float b, float t) {
		return a + (b - a) * t;
	}

	Vector2 lerp(Vector2 a, Vector2 b, float t) {
		t = std::clamp(t, 0.0f, 1.0f);
		return a + (b - a) * t;
	}

	Vector2 unclampedLerp(Vector2 a, Vector2 b, float t) {
		return a + (b - a) * t;
	}
}