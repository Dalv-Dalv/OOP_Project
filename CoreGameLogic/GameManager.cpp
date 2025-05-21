#include <iostream>

#include "GameManager.h"

#include "Component.h"
#include "GameCamera.h"
#include "GameObject.h"
#include <raylib.h>

#include "InputManager.h"
#include "../GameLogic/Player/Inventory/InventoryManager.h"
#include "../GameLogic/UI/UIManager.h"

GameManager* GameManager::instance = nullptr;
ActionEvent<> GameManager::onGameClose = ActionEvent<>();

GameManager* GameManager::GetInstance() {
	if(instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}

void GameManager::InitializeRenderPipeline() {
	renderPipeline = RenderPipeline::GetInstance();
	scenePass = RenderPass::Create(windowWidth, windowHeight, 2, true);
}


void GameManager::Initialize(int windowWidth, int windowHeight, bool startInFullscreen, bool vsync) {
	if(vsync)SetTargetFPS(144);
	else SetTargetFPS(0);

	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
	if(!vsync) SetConfigFlags(FLAG_VSYNC_HINT);
	if(startInFullscreen) SetConfigFlags(FLAG_FULLSCREEN_MODE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->screenRect = Rectangle(0, 0, GameManager::GetWindowWidth(), GameManager::GetWindowHeight());

	InitWindow(windowWidth, windowHeight, "Proiect POO");
	if(!vsync) ClearWindowState(FLAG_VSYNC_HINT); // After InitWindow


	GameObject* camera = new GameObject({0, 0});
	camera->AddComponent(new GameCamera());

	InitializeRenderPipeline();
	UIManager::Initialize();
	InventoryManager::Initialize();

	InitAudioDevice();
}

void GameManager::StartGameLoop() {
	Awake();

	while(!WindowShouldClose()) {
		float deltaTime = GetFrameTime();

		Update(deltaTime);
		InputManager::ResetCapture();

		// If any object was marked for deletion this frame, delete it
		GameObject::DeleteAllMarked();

		renderPipeline->Render();
	}

	onGameClose();

	cout << "DELETING ALL GAME OBJECTS\n";
	while (!GameObject::gameObjects.empty()) {
		delete *GameObject::gameObjects.begin();
	}
	GameObject::gameObjects.clear();

	UIManager::Dispose();

	CloseWindow();
}

void GameManager::SubscribeOnGameClose(const function<void()>& subscriber) {
	onGameClose += subscriber;
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

void GameManager::Dispose() {
	AssetManager::UnloadAll();
	CloseAudioDevice();
	delete InventoryManager::instance;
	delete instance;
}


// Getters
int GameManager::GetWindowWidth() { return instance->windowWidth; }
int GameManager::GetWindowHeight() { return instance->windowHeight; }
Rectangle GameManager::GetScreenRect() { return instance->screenRect; }
shared_ptr<RenderPass> GameManager::GetScenePass() { return instance->scenePass; }
