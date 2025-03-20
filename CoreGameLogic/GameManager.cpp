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
int GameManager::GetWindowWidth() { return instancePtr->windowWidth; }
int GameManager::GetWindowHeight() { return instancePtr->windowHeight; }


void GameManager::Initialize(int windowWidth, int windowHeight, bool startInFullscreen) {
	SetTargetFPS(144);
	SetConfigFlags(FLAG_VSYNC_HINT);
	SetConfigFlags(FLAG_MSAA_4X_HINT);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	InitWindow(windowWidth, windowHeight, "Proiect POO");

	if(startInFullscreen) ToggleBorderlessWindowed();

	Awake();

	while(!WindowShouldClose()) {
		BeginDrawing();

		Update();

		EndDrawing();
	}
	CloseWindow();
}

void GameManager::Awake() {
	for(auto element : Component::components) {
		element->Awake();
	}
}

void GameManager::Update() {
	float deltaTime = GetFrameTime();

	for(auto element : Component::components) {
		element->Update(deltaTime);
	}
}
