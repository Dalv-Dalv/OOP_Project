#include "PlayerRenderer.h"

#include <iostream>

#include "raylib.h"
#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../Collisions/CollisionInfo.h"
#include "../Map/Terrain.h"

void PlayerRenderer::Awake() {
	GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});
}

void PlayerRenderer::Update(float deltaTime) { }

void PlayerRenderer::Render() const {
	DrawCircle(gameObject->position.x, gameObject->position.y, size, color);
}

PlayerRenderer::PlayerRenderer(float size, Color color) : size(size), color(color) {}

PlayerRenderer::~PlayerRenderer() { }

void PlayerRenderer::Print(std::ostream &os) const {
	os << "PlayerRenderer";
}

