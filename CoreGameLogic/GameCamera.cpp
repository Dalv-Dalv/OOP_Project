#include "GameCamera.h"

#include "GameManager.h"
#include "GameObject.h"

GameCamera* GameCamera::instance = nullptr;

GameCamera::GameCamera() : internalCamera() {
	instance = this;
	internalCamera.target = {0, 0};
	internalCamera.offset = Vector2(GameManager::GetWindowWidth() / 2, GameManager::GetWindowHeight() / 2);
	internalCamera.rotation = 0.0f;
	internalCamera.zoom = 1.0f;
}

void GameCamera::Awake() {
	internalCamera.offset = gameObject->position;
}
void GameCamera::Update(float deltaTime) {
	internalCamera.offset = gameObject->position;
}



GameCamera* GameCamera::GetActiveCamera() {
	return instance;
}

// Getters
const Camera2D& GameCamera::GetCamera2D() const { return internalCamera; }
