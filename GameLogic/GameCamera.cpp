#include "GameCamera.h"

#include "raylib.h"
#include "../CoreGameLogic/GameManager.h"

GameCamera* GameCamera::mainCamera = nullptr;

void GameCamera::Awake() {

}

void GameCamera::Update(float deltaTime) {

}

GameCamera::GameCamera() : camera() {
	int width = GameManager::GetWindowWidth();
	int height = GameManager::GetWindowHeight();

	camera.offset = Vector2(width / 2, height / 2);
	camera.target = Vector2(0, 0);

	camera.rotation = 0;
	camera.zoom = 1;
}
GameCamera::~GameCamera() {
	if(mainCamera == this) mainCamera = nullptr;
}


Camera2D GameCamera::GetInternalCamera2D() const {
	return camera;
}

void GameCamera::MakeMain() {
	mainCamera = this;
}


GameCamera* GameCamera::GetMainCamera() {
	return mainCamera;
}
