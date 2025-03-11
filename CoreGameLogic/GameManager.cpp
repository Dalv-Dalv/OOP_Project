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
	InitWindow(windowWidth, windowHeight, "Proiect POO");
	SetTargetFPS(144);
	SetConfigFlags(FLAG_VSYNC_HINT);

	if(startInFullscreen) ToggleFullscreen();
	//else ToggleBorderlessWindowed();

	Start();

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Pog", 20, 20, 30, ORANGE);

		Update();

		EndDrawing();
	}
	CloseWindow();
}

void GameManager::Start() {
	for(auto element : Component::gameElements) {
		element->Start();
	}
}

void GameManager::Update() {
	for(auto element : Component::gameElements) {
		element->Update();
	}
}
