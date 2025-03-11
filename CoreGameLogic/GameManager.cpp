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

void GameManager::Initialize(int windowWidth, int windowHeight, bool startInFullscreen) {
	SetTargetFPS(144);
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	InitWindow(windowWidth, windowHeight, "Proiect POO");

	if(startInFullscreen) ToggleBorderlessWindowed();

	Start();

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Pog", 20, 20, 30, ORANGE);

		DrawPolyLines({100, 100},6, 50, 0, RED);

		Update();

		EndDrawing();
	}
	CloseWindow();
}

void GameManager::Start() {
	for(auto element : Component::components) {
		element->Start();
	}
}

void GameManager::Update() {
	for(auto element : Component::components) {
		element->Update();
	}
}
