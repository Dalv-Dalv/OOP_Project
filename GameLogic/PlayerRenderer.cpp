#include "PlayerRenderer.h"

#include <iostream>

#include "raylib.h"
#include "../CoreGameLogic/GameManager.h"
#include "../CoreGameLogic/GameObject.h"

void PlayerRenderer::Awake() {
	renderID = GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});
}

void PlayerRenderer::Update(float deltaTime) { }

void PlayerRenderer::Render() const {
	DrawCircle(gameObject->position.x, gameObject->position.y, size, color);
}

PlayerRenderer::PlayerRenderer(float size, Color color) : size(size), color(color) {}

PlayerRenderer::~PlayerRenderer() {
	GameManager::GetScenePass()->RemoveFunction(renderID);
}

void PlayerRenderer::Print(std::ostream &os) const {
	os << "PlayerRenderer";
}

