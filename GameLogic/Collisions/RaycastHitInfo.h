#pragma once
#include <raylib.h>

struct RaycastHitInfo {
	float hitDistance;
	bool hasHit;
	Vector2 origin;
	Vector2 normal; // Normalized
	Vector2 hitPoint;

	RaycastHitInfo(float hitDistance = 0, bool hasHit = false, const Vector2 origin = {0, 0}, const Vector2 normal = {0, 0}, const Vector2 hitPoint = {0, 0}) : hitDistance(hitDistance), hasHit(hasHit), origin(origin), normal(normal), hitPoint(hitPoint) {}

	operator bool() const { return hasHit; }
	bool operator<(const RaycastHitInfo& other) const {
		if(!hasHit && other.hasHit) return false;
		if(hasHit && !other.hasHit) return true;
		return hitDistance < other.hitDistance;
	}
	bool operator>(const RaycastHitInfo& other) const {
		if(!hasHit && other.hasHit) return true;
		if(hasHit && !other.hasHit) return false;
		return hitDistance > other.hitDistance;
	}
	bool operator==(const RaycastHitInfo& other) const {
		return hasHit == other.hasHit && hitDistance == other.hitDistance && normal.x == other.normal.x && normal.y == other.normal.y;
	}
};