#pragma once
#include <raylib.h>

struct CollisionInfo {
	float signedDist;
	Vector2 normal; // With distance included
	Vector2 closestPoint; //FOR DEBUG TODO: REMOVE LATER

	CollisionInfo() : signedDist(1000000000), normal(0, 0), closestPoint(0, 0) {}
	CollisionInfo(float signedDist, const Vector2 normal) : signedDist(signedDist), normal(normal), closestPoint(0, 0) {}
	CollisionInfo(float signedDist, const Vector2 normal, const Vector2 closestPoint) : signedDist(signedDist), normal(normal), closestPoint(closestPoint) {}

	operator bool() const { return signedDist; }
	bool operator<(const CollisionInfo& other) const {
		return abs(signedDist) < abs(other.signedDist);
	}
	bool operator>(const CollisionInfo& other) const {
		return abs(signedDist) > abs(other.signedDist);
	}
	bool operator==(const CollisionInfo& other) const {
		return (normal.x * normal.x + normal.y * normal.y) == (other.normal.x * other.normal.x + other.normal.y * normal.y);
	}
};
