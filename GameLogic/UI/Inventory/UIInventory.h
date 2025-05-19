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

	bool isHoveredOver = false;
	int prevHoverIndex = -1;

	bool isBeingClickedOn = false; float clickTime = -1.0f;
	bool isDragging = false; int clickIndex = -1;

	int GetSlotIndexFromPos(Vector2 pos) const;
	void HandleClickOrDrag();

public:
	UIInventory(float slotSize, float spacing);

	void HandleActiveChanged(int index);
	void HandleItemsSwapped(int i1, int i2);
	void HandleOnItemAdded(IItem* item, int index);
	void HandleNrOfLockedSlotsChanged(int nrOfLockedSlots);

	void Draw() override;
	void OnClicked() override;
	void OnMouseUp() override;
	void OnHover() override;
	bool ContainsPoint(Vector2 point) const override;
};
