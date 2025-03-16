#include <cmath>

#include "Vector2Utils.h"
#include "raylib.h"

namespace GameUtilities {
	// The famous fast inverse sqrt
	float fastInverseSqrt(float number) {
		const float threehalfs = 1.5F;

		float x2 = number * 0.5F;
		float y = number;

		// evil floating point bit level hacking
		long i = *(long*)&y;

		// value is pre-assumed
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;

		// 1st iteration
		y = y * (threehalfs - (x2 * y * y));

		// 2nd iteration, this can be removed
		y = y * (threehalfs - (x2 * y * y));

		return y;
	}

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
		return {lhs.x + rhs.x, lhs.y + rhs.y};
	}
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
		return {lhs.x - rhs.x, lhs.y - rhs.y};
	}

	Vector2 operator*(const Vector2& v, float scalar) {
		return {v.x * scalar, v.y * scalar};
	}
	Vector2 operator*(float scalar, const Vector2& v) {
		return {v.x * scalar, v.y * scalar};
	}
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
		return {lhs.x * rhs.x, lhs.y * rhs.y};
	}


	Vector2 operator/(const Vector2& v, float scalar) {
		return {v.x / scalar, v.y / scalar};
	}
	Vector2 operator/(float scalar, const Vector2& v) {
		return {v.x * scalar, v.y * scalar};
	}

	void operator+=(Vector2& lhs, const Vector2& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
	}
	void operator-=(Vector2& lhs, const Vector2& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& v) {
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	float V2Length(const Vector2& v) {
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	Vector2 V2Normalized(const Vector2& v) {
		return v * fastInverseSqrt(v.x * v.x + v.y * v.y);
	}
}
