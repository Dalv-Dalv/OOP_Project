#include <cmath>

#include "Vector2Utils.h"
#include "raylib.h"

namespace GameUtilities {
	// The famous fast inverse sqrt
	float fastInverseSqrt(float number) {
		int32_t i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y  = number;
		i  = *(int32_t*)&y;       // Ensure correct size
		i  = 0x5f3759df - (i >> 1);
		y  = *(float*)&i;
		y  = y * (threehalfs - (x2 * y * y));

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
	void operator*=(Vector2& lhs, float scalar) {
		lhs.x *= scalar;
		lhs.y *= scalar;
	}
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
		return {lhs.x * rhs.x, lhs.y * rhs.y};
	}
	void operator*=(Vector2& lhs, const Vector2& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
	}



	Vector2 operator/(const Vector2& v, float scalar) {
		return {v.x / scalar, v.y / scalar};
	}
	Vector2 operator/(float scalar, const Vector2& v) {
		return {v.x * scalar, v.y * scalar};
	}
	void operator/=(Vector2& lhs, float scalar) {
		lhs.x /= scalar;
		lhs.y /= scalar;
	}

	Vector2 operator/(const Vector2& lhs, const Vector2& rhs) {
		return {lhs.x / rhs.x, lhs.y / rhs.y};
	}
	void operator/=(Vector2& lhs, const Vector2& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
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
