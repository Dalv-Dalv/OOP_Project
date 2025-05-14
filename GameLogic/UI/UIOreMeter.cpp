#include "UIOreMeter.h"

#include <iostream>

#include "../../CoreGameLogic/GameManager.h"

std::optional<Shader> UIOreMeter::oreMeterShader = std::nullopt;
std::optional<Texture2D> UIOreMeter::whiteTex = std::nullopt;
int UIOreMeter::timeLoc = -1, UIOreMeter::meterSizeLoc = -1;
int UIOreMeter::metersAvailable = 0;

UIOreMeter::UIOreMeter(int capacity) : UIElement(true) {
	if(oreMeterShader == std::nullopt) {
		oreMeterShader = LoadShader(0, "Shaders/oreMeterShader.frag");
		timeLoc = GetShaderLocation(oreMeterShader.value(), "time");
		meterSizeLoc = GetShaderLocation(oreMeterShader.value(), "meterSize");

		int screenSizeLoc = GetShaderLocation(oreMeterShader.value(), "screenSize");
		Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
		SetShaderValue(oreMeterShader.value(), screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);
	}
	if(whiteTex == std::nullopt) {
		whiteTex = LoadTexture("Textures/White.png");
	}

	meterSize = Vector2(40, capacity * 100);
	rect = Rectangle( 30 + metersAvailable * 60, 400, 40, meterSize.y);

	metersAvailable++;
}

void UIOreMeter::Draw() {
	float time = GetTime();

	// BeginShaderMode(oreMeterShader.value());
	// 	SetShaderValue(oreMeterShader.value(), timeLoc, &time, SHADER_UNIFORM_FLOAT);
	// 	SetShaderValue(oreMeterShader.value(), meterSizeLoc, &meterSize, SHADER_UNIFORM_FLOAT);
	// 	DrawTexturePro(whiteTex.value(), Rectangle(0,0,1,1), rect, Vector2(0,0), 0, WHITE);
	// EndShaderMode();

	DrawRectanglePro(rect, {0,0}, 0, testState);

	testState = Color(255, 255, 255, 255);
}

void UIOreMeter::ChangeCapacity(int newCapacity) {
	meterSize.y = newCapacity * 100;
	rect.height = meterSize.y;
}

void UIOreMeter::OnMouseDown() {
	testState = Color(0,0,255, 255);
}
void UIOreMeter::OnMouseUp() {
	testState = Color(0,255,0, 255);
}
void UIOreMeter::OnHover() {
	testState = Color(255, 0, 0, 255);
}






