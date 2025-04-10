#include <iostream>

#include "GameManager.h"

#include "Component.h"
#include "GameCamera.h"
#include "GameObject.h"
#include <raylib.h>

GameManager* GameManager::instance = nullptr;
ActionEvent<> GameManager::onGameClose = ActionEvent<>();

GameManager* GameManager::GetInstance() {
	if(instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}

//TODO: CLEAN THIS UP
// RenderTexture2D& GameManager::GetActiveRenderTexture() {
// 	return instance->renderTexture;
// }
// void GameManager::SetActiveRenderTexture(RenderTexture2D& renderTexture) {
// 	instance->renderTexture = renderTexture;
// }

void GameManager::InitializeRenderPipeline() {
	renderPipeline = RenderPipeline::GetInstance();
	scenePass = RenderPass::Create(windowWidth, windowHeight, 2, true);
	uiPass = RenderPass::Create(windowWidth, windowHeight, 100, false);
	uiPass->AddFunction([](RenderTexture2D& prev){
		RenderPipeline::DrawTextureFullScreen(prev);
		float fps = GetFPS();
		DrawText(TextFormat("%.1f", fps), 0, 0, 25, GREEN);
	});
}


void GameManager::Initialize(int windowWidth, int windowHeight, bool startInFullscreen, bool vsync) {
	if(vsync)SetTargetFPS(144);
	else SetTargetFPS(0);

	if(!vsync) SetConfigFlags(FLAG_VSYNC_HINT); // Before InitWindow
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->screenRect = Rectangle(0, 0, GameManager::GetWindowWidth(), GameManager::GetWindowHeight());

	InitWindow(windowWidth, windowHeight, "Proiect POO");
	if(!vsync) ClearWindowState(FLAG_VSYNC_HINT); // After InitWindow

	if(startInFullscreen) ToggleFullscreen();

	GameObject* camera = new GameObject({0, 0});
	camera->AddComponent(new GameCamera());

	InitializeRenderPipeline();
}

void GameManager::StartGameLoop() {
	Awake();

	while(!WindowShouldClose()) {
		float deltaTime = GetFrameTime();

		Update(deltaTime);

		renderPipeline->Render();
	}

	onGameClose();

	cout << "DELETING ALL GAME OBJECTS\n";
	while (!GameObject::gameObjects.empty()) {
		delete *GameObject::gameObjects.begin();
	}
	GameObject::gameObjects.clear();

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

// Getters
int GameManager::GetWindowWidth() { return instance->windowWidth; }
int GameManager::GetWindowHeight() { return instance->windowHeight; }
Rectangle GameManager::GetScreenRect() { return instance->screenRect; }
shared_ptr<RenderPass> GameManager::GetScenePass() { return instance->scenePass; }
shared_ptr<RenderPass> GameManager::GetUIPass() { return instance->uiPass; }
