#include "GameCamera.h"

#include <iostream>

#include "GameManager.h"
#include "GameObject.h"
#include "../Utilities/Vector2Utils.h"

using namespace GameUtilities;

GameCamera* GameCamera::instance = nullptr;

GameCamera::GameCamera() : internalCamera() {
	instance = this;
	internalCamera.target = {0, 0};
	internalCamera.offset = Vector2(GameManager::GetWindowWidth() / 2, GameManager::GetWindowHeight() / 2);
	internalCamera.rotation = 0.0f;
	internalCamera.zoom = 1.0f;
}

void GameCamera::Awake() {
	internalCamera.target = gameObject->position;
}
void GameCamera::Update(float deltaTime) {
	internalCamera.target = gameObject->position;

	// float speed = 100;
	//
	// if(IsKeyDown(KEY_SPACE)) {
	// 	internalCamera.offset.y -= deltaTime * speed;
	// }else if(IsKeyDown(KEY_LEFT_CONTROL)) {
	// 	internalCamera.offset.y += deltaTime * speed;
	// }
	//
	// if(IsKeyDown(KEY_E)) {
	// 	internalCamera.offset.x -= deltaTime * speed;
	// }else if(IsKeyDown(KEY_Q)) {
	// 	internalCamera.offset.x += deltaTime * speed;
	// }
}

Vector2 GameCamera::ScreenToWorldCoords(Vector2 screenPos) {
	auto cam = GetActiveCamera()->internalCamera;
	Vector2 worldPos = {screenPos.x + cam.target.x - cam.offset.x, screenPos.y + cam.target.y - cam.offset.y};
	return worldPos;
}

void GameCamera::Print(std::ostream &os) const {
	os << "GameCamera" << std::endl;
}

// Getters
GameCamera* GameCamera::GetActiveCamera() { return instance; }
float GameCamera::GetZoom() { return instance->internalCamera.zoom; }

Vector2 GameCamera::GetOffsetPos() const { return internalCamera.offset; }
const Camera2D& GameCamera::GetCamera2D() const { return internalCamera; }
Rectangle GameCamera::GetWorldSpaceScreenRect() const {
	return {gameObject->position.x - internalCamera.offset.x, gameObject->position.y - internalCamera.offset.y, static_cast<float>(GameManager::GetWindowWidth()), static_cast<float>(GameManager::GetWindowHeight())};
}


