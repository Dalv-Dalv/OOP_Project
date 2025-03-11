#include "PlayerRenderer.h"

#include <iostream>

#include "../CoreGameLogic/GameObject.h"

void PlayerRenderer::Start() {
	std::cout << "Player start" << std::endl;
}

void PlayerRenderer::Update() {
	std::cout << "Player update" << std::endl;

	auto pos = gameObject.position;
	DrawCircle(pos.x, pos.y, 50.0f, BLUE);
}
