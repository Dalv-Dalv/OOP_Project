#include "PlayerRenderer.h"

#include <iostream>

#include "raylib.h"
#include "../CoreGameLogic/GameObject.h"

void PlayerRenderer::Start() {
	std::cout << "Player renderer start";
}

void PlayerRenderer::Update(float deltaTime) {
	auto pos = gameObject->position;
	DrawCircle(pos.x, pos.y, size, color);
}

PlayerRenderer::PlayerRenderer(float size, Color color) : size(size), color(color) {}

