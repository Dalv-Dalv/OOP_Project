#pragma once
#include <raylib.h>

class V2SecondOrderDynamics {
private:
	Vector2 xp, y, yd;
	float k1, k2, k3;
public:
	V2SecondOrderDynamics(float f, float z, float r, Vector2 x0);
	void ChangeParameters(float f, float z, float r);

	Vector2 Update(float T, Vector2 x, Vector2 xd);
};