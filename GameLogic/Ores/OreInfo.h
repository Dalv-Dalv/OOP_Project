#pragma once

struct OreInfo {
	float quantities[9] {0.0f};

	void AddOreFromMap(float oreType, float oreValue);

	float& operator[](int index);
};
