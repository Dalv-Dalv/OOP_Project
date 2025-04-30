#pragma once
#include <raylib.h>

struct CollisionInfo {
	float signedDist;
	Vector2 normal; // Normalized
	Vector2 closestPoint;

	CollisionInfo(float signedDist = 1000000000, const Vector2 normal = {0, 0}, const Vector2 closestPoint = {0, 0}) : signedDist(signedDist), normal(normal), closestPoint(closestPoint) {}

	operator bool() const { return signedDist; }
	bool operator<(const CollisionInfo& other) const {
		if(abs(abs(signedDist) - abs(other.signedDist)) < 0.01f) {
			return other.signedDist < 0;
		}
		return abs(signedDist) < abs(other.signedDist);
	}
	bool operator>(const CollisionInfo& other) const {
		return other < *this;
	}
	bool operator==(const CollisionInfo& other) const {
		return (normal.x * normal.x + normal.y * normal.y) == (other.normal.x * other.normal.x + other.normal.y * normal.y);
	}
};
