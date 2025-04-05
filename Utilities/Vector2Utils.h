#ifndef VECTOR2UTILS_H
#define VECTOR2UTILS_H
#include <ostream>

#include "raylib.h"

namespace GameUtilities {
	// The famous fast inverse sqrt
	float fastInverseSqrt(float number);

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

	Vector2 operator*(const Vector2& v, float scalar);
	Vector2 operator*(float scalar, const Vector2& v);
	void operator*=(Vector2& lhs, float scalar);
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs);
	void operator*=(Vector2& lhs, const Vector2& rhs);

	Vector2 operator/(const Vector2& v, float scalar);
	Vector2 operator/(float scalar, const Vector2& v);
	void operator/=(Vector2& lhs, float scalar);
	Vector2 operator/(const Vector2& lhs, const Vector2& rhs);
	void operator/=(Vector2& lhs, const Vector2& rhs);

	void operator+=(Vector2& lhs, const Vector2& rhs);
	void operator-=(Vector2& lhs, const Vector2& rhs);



	std::ostream& operator<<(std::ostream& os, const Vector2& v);

	float V2Length(const Vector2& v);

	Vector2 V2Normalized(const Vector2& v);
}


#endif //VECTOR2UTILS_H
