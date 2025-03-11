#include "V2SecondOrderDynamics.h"

#include <algorithm>

#include "../../Utilities/Vector2Utils.h"

using namespace GameUtilities;

V2SecondOrderDynamics::V2SecondOrderDynamics(float f, float z, float r, Vector2 x0) {
	ChangeParameters(f, z, r);

	xp = y = x0;
	yd = {0, 0};
}

void V2SecondOrderDynamics::ChangeParameters(float f, float z, float r) {
	k1 = z / (PI * f);

	k2 = 2 * PI * f;
	k2 = fastInverseSqrt(k2 * k2);

	k3 = r * z / (2 * PI * f);
}

Vector2 V2SecondOrderDynamics::Update(float T, Vector2 x, Vector2 xd) {
	float k2stable = std::max(k2, 1.1f * (T * T / 4 + T * k1 / 2));
	y = y + T * yd;
	yd = yd + T * (x + k3 * xd - y - k1 * yd) / k2stable;
	return y;
}
