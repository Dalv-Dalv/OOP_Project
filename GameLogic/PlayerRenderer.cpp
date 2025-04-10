#include "PlayerRenderer.h"

#include <iostream>

#include "raylib.h"
#include "../CoreGameLogic/GameManager.h"
#include "../CoreGameLogic/GameObject.h"
#include "Collisions/CollisionInfo.h"
#include "Map/Terrain.h"

void PlayerRenderer::Awake() {
	renderID = GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});
}

void PlayerRenderer::Update(float deltaTime) { }

void PlayerRenderer::Render() const {

	auto tes = Terrain::CheckCollisions(gameObject->position, size);

	if(tes.signedDist < size) {
		DrawCircle(gameObject->position.x, gameObject->position.y, size, RED);
	} else {
		DrawCircle(gameObject->position.x, gameObject->position.y, size, color);
	}
}

PlayerRenderer::PlayerRenderer(float size, Color color) : size(size), color(color) {}

PlayerRenderer::~PlayerRenderer() {
	GameManager::GetScenePass()->RemoveFunction(renderID);
}

void PlayerRenderer::Print(std::ostream &os) const {
	os << "PlayerRenderer";
}

