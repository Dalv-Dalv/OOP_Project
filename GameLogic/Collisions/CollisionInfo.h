#pragma once
#include <raylib.h>

struct CollisionInfo {
	const bool hasCollided;
	const Vector2 normal; // With distance included

	CollisionInfo(const bool collided, const Vector2 normal) : hasCollided(collided), normal(normal) {}

	operator bool() const { return hasCollided; }
};
