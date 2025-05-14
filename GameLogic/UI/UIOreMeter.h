#pragma once
#include <optional>

#include "UIElement.h"

class UIOreMeter : public UIElement {
private:
	static std::optional<Shader> oreMeterShader;
	static int timeLoc, meterSizeLoc;

	static std::optional<Texture2D> whiteTex;

	static int metersAvailable;

	Vector2 meterSize;

	Color testState = Color(255, 0, 255, 255);

public:
	UIOreMeter(int capacity);

	void Draw() override;
	void OnMouseDown() override;
	void OnMouseUp() override;
	void OnHover() override;

	void ChangeCapacity(int newCapacity);
};
