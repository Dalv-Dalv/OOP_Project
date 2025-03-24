#include <iostream>

#include "GameManager.h"

#include "Component.h"
#include "raylib.h"

GameManager* GameManager::instancePtr = nullptr;

GameManager* GameManager::GetInstance() {
	if(instancePtr == nullptr) {
		instancePtr = new GameManager();
	}
	return instancePtr;
}
RenderTexture2D& GameManager::GetActiveRenderTexture() {
	return instancePtr->renderTexture;
}
void GameManager::SetActiveRenderTexture(RenderTexture2D& renderTexture) {
	instancePtr->renderTexture = renderTexture;
}


int GameManager::GetWindowWidth() { return instancePtr->windowWidth; }
int GameManager::GetWindowHeight() { return instancePtr->windowHeight; }


void GameManager::Initialize(int windowWidth, int windowHeight, bool startInFullscreen) {
	SetTargetFPS(6000);
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	InitWindow(windowWidth, windowHeight, "Proiect POO");

	if(startInFullscreen) ToggleFullscreen();

	renderTexture = LoadRenderTexture(windowWidth, windowHeight);
	// Shader test = LoadShader(0, TextFormat("../Shaders/posTestShader.frag", 430));
	// int screenLoc = GetShaderLocation(test, "screenSize");
	// int posLoc = GetShaderLocation(test, "position");
	// int timeLoc = GetShaderLocation(test, "time");
	// int testLoc = GetShaderLocation(test, "testParam");
	// Vector2 screenSize(windowWidth, windowHeight);
	// SetShaderValue(test, screenLoc, &screenSize, SHADER_UNIFORM_VEC2);

	Awake();

	while(!WindowShouldClose()) {
		float deltaTime = GetFrameTime();

		// float t = GetTime();
		// SetShaderValue(test, timeLoc, &t, SHADER_UNIFORM_FLOAT);
		//
		// BeginShaderMode(test);
		// float unitSize = 300.0;
		// for(int y = 0; y < 3; y++) {
		// 	for(int x = 0; x < 3; x++) {
		// 		Vector2 pos(x * unitSize, y * unitSize);
		//
		// 		float testParam = (x + y + 1) % 2;
		//
		// 		SetShaderValue(test, testLoc, &testParam, SHADER_ATTRIB_FLOAT);
		// 		DrawRectangle(pos.x + 1, pos.y + 1, unitSize - 2, unitSize - 2, WHITE);
		// 	}
		// }
		// EndShaderMode();

		Update(deltaTime);

		BeginDrawing();
			ClearBackground(DARKGRAY);
			DrawTextureRec(GetActiveRenderTexture().texture,
				{0, 0, (float)windowWidth, (float)windowHeight},
				{0, 0},
				WHITE);

			float fps = GetFPS();
			DrawText(TextFormat("%.1f", fps), 0, 0, 25, GREEN);
		EndDrawing();
	}

	OnGameClose();

	CloseWindow();
}

void GameManager::Awake() {
	for(auto element : Component::components) {
		element->Awake();
	}
}

void GameManager::Update(float deltaTime) {
	for(auto element : Component::components) {
		element->Update(deltaTime);
	}
}

void GameManager::OnGameClose() {
	for(auto element : Component::components) {
		element->OnGameClose();
	}
}
