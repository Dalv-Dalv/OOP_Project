#pragma once
#include <iostream>
#include <optional>

#include "../../Player/Inventory/IItem.h"

struct UIInventorySlot {
	bool locked = false;
	std::optional<Texture2D> icon;
	float highlight = 0, roundness = 1, size = 1;
	float targetHighlight = 0, targetRoundness = 1.4, targetSize = 1;

	void UpdateValues(float deltaTime) {
		highlight = std::lerp(highlight, targetHighlight, deltaTime * 15);
		roundness = std::lerp(roundness, targetRoundness, deltaTime * 15);
		size = std::lerp(size, targetSize, deltaTime * 8);
	}
};
