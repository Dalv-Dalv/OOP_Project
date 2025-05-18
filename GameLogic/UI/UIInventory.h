#pragma once
#include "UIElement.h"

class UIInventory : UIElement {
public:
	void Draw() override;
	void OnMouseDown() override;
	void OnMouseUp() override;
	void OnHover() override;
};
