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

	Vector2 worldSize = {1920, 1080};

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		for (int x = 0; x < worldSize.x; x += 100)
			DrawLine(x, 0, x, worldSize.y, LIGHTGRAY);
		for (int y = 0; y < worldSize.y; y += 100)
			DrawLine(0, y, worldSize.x, y, LIGHTGRAY);

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
	float deltaTime = GetFrameTime();

	DrawText(TextFormat("%.1f", 1 / deltaTime), 15, 15, 25, DARKGREEN);

	for(auto element : Component::components) {
		element->Update(deltaTime);
	}
}
