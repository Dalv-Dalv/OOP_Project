#pragma once
#include "UIInventorySlot.h"
#include "../UIElement.h"

class UIInventory : UIElement {
private:
	std::vector<UIInventorySlot> slots;
	const Shader& slotShader; int indexLoc, timeLoc, iconLoc, highlightLoc, roundnessLoc;
	Texture2D whiteTex, transparentTex;

	float slotSize, spacing;

	int activeIndex = 0;

public:
	UIInventory(float slotSize, float spacing);

	void HandleActiveChanged(int index);
	void HandleOnItemAdded(IItem* item, int index);
	void HandleNrOfLockedSlotsChanged(int nrOfLockedSlots);

	void Draw() override;
	void OnMouseDown() override;
	void OnMouseUp() override;
	void OnHover() override;
};
