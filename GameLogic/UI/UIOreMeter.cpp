#include "UIOreMeter.h"

#include <iostream>

#include "../../CoreGameLogic/AssetManager.h"
#include "../../CoreGameLogic/GameManager.h"

std::optional<Shader> UIOreMeter::oreMeterShader = std::nullopt;
std::optional<Texture2D> UIOreMeter::whiteTex = std::nullopt, UIOreMeter::oreColors = std::nullopt;
int UIOreMeter::timeLoc = -1, UIOreMeter::meterSizeLoc = -1, UIOreMeter::fillLoc = -1, UIOreMeter::oreTypeLoc = -1, UIOreMeter::oreColorsLoc;
int UIOreMeter::metersAvailable = 0;

UIOreMeter::UIOreMeter(IOreContainer* oreContainer, int capacity) : UIElement(true), oreContainer(oreContainer) {
	if(oreMeterShader == std::nullopt) {
		oreMeterShader = AssetManager::LoadShader("Shaders/oreMeterShader.frag");
		timeLoc = GetShaderLocation(oreMeterShader.value(), "time");
		fillLoc = GetShaderLocation(oreMeterShader.value(), "fill");
		meterSizeLoc = GetShaderLocation(oreMeterShader.value(), "meterSize");
		oreTypeLoc = GetShaderLocation(oreMeterShader.value(), "oreType");
		oreColorsLoc = GetShaderLocation(oreMeterShader.value(), "oreColors");

		int screenSizeLoc = GetShaderLocation(oreMeterShader.value(), "screenSize");
		Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
		SetShaderValue(oreMeterShader.value(), screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
	}
	if(whiteTex == std::nullopt) {
		whiteTex = AssetManager::LoadTexture("Textures/White.png");
	}
	if(oreColors == std::nullopt) {
		oreColors = AssetManager::LoadTexture("Textures/OreColors.png");
	}

	meterSize = Vector2(40, capacity * 100);
	rect = Rectangle( 30 + metersAvailable * 60, 1000 - meterSize.y, 40, meterSize.y);

	metersAvailable++;
}
UIOreMeter::~UIOreMeter() {
	metersAvailable--;
}


void UIOreMeter::Draw() {
	float time = GetTime();

	BeginShaderMode(oreMeterShader.value());
		SetShaderValue(oreMeterShader.value(), timeLoc, &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(oreMeterShader.value(), fillLoc, &fill, SHADER_UNIFORM_FLOAT);
		SetShaderValue(oreMeterShader.value(), oreTypeLoc, &oreType, SHADER_UNIFORM_FLOAT);
		SetShaderValueTexture(oreMeterShader.value(), oreColorsLoc, oreColors.value());
		SetShaderValue(oreMeterShader.value(), meterSizeLoc, &meterSize, SHADER_UNIFORM_VEC2);
		DrawTexturePro(whiteTex.value(), Rectangle(0,0,1,1), rect, Vector2(0,0), 0, WHITE);
	EndShaderMode();

	// DrawRectanglePro(rect, {0,0}, 0, testState);

	testState = Color(255, 255, 255, 255);
}


void UIOreMeter::UpdateFill(float fill01) {
	fill = fill01;
}
void UIOreMeter::ChangeOreType(float newType) {
	oreType = newType;
}
void UIOreMeter::ChangeCapacity(float newCapacity) {
	meterSize = Vector2(40, newCapacity * 100);
	rect = Rectangle( 30 + metersAvailable * 60, 1000 - meterSize.y, 40, meterSize.y);
}
void UIOreMeter::ChangeScaledCapacity(float scaledCapacity) {
	ChangeCapacity(scaledCapacity / 30);
}

void UIOreMeter::OnMouseDown() {}
void UIOreMeter::OnMouseUp() {}
void UIOreMeter::OnHover() {}






