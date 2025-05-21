#pragma once
#include <optional>

#include "UIElement.h"
#include "../Ores/IOreContainer.h"

class UIOreMeter : public UIElement {
private:
	static std::optional<Shader> oreMeterShader;
	static int timeLoc, meterSizeLoc, fillLoc, oreTypeLoc, oreColorsLoc;
	float fill = 0, oreType = -1;

	static std::optional<Texture2D> whiteTex, oreColors;

	static int metersAvailable;


	Vector2 meterSize;

	IOreContainer* oreContainer;
	Color testState = Color(255, 0, 255, 255);


public:
	UIOreMeter(IOreContainer* oreContainer, int capacity);
	~UIOreMeter();

	void Draw() override;
	void OnMouseDown() override;
	void OnMouseUp() override;
	void OnHover() override;


	void UpdateFill(float fill01);
	void ChangeOreType(float newType);
	void ChangeCapacity(float newCapacity);
	void ChangeScaledCapacity(float scaledCapacity);
};
