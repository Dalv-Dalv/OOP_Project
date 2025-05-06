#include "PulsingOrb.h"

#include <iostream>

#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/Vector2Utils.h"
#include "../Map/Terrain.h"

using namespace GameUtilities;

void PulsingOrb::Awake() {}

void PulsingOrb::Update(float deltaTime) {
	size = startV + abs(sin(GetTime() - startTime)) * sizeIncrease;
	auto collision = Terrain::CheckCollisions(gameObject->position, size);

	velocity.y += 9.81 * 100 * deltaTime;

	if(collision.signedDist < 0 || collision.signedDist < size) {
		auto dir = collision.closestPoint - gameObject->position;
		dir = dir * fastInverseSqrt(dir.x*dir.x + dir.y*dir.y);
		dir *= dir.x * collision.normal.x + dir.y * collision.normal.y;

		gameObject->position = collision.closestPoint - dir * size;
		velocity = Reflect(velocity, collision.normal) * 0.95;

		// Terrain::GetActiveTerrain()->MineAt(gameObject->position, 4, 3000, deltaTime);
	}

	gameObject->position += velocity * deltaTime;
}

void PulsingOrb::Render() const {
	DrawCircle(gameObject->position.x, gameObject->position.y, size, color);
}

PulsingOrb::PulsingOrb(float startV, float sizeIncrease, Vector2 startVelocity, Color color) : startV(startV), sizeIncrease(sizeIncrease), velocity(startVelocity), color(color) {
	if(GameManager::GetScenePass() == nullptr) return;
	GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});

	startTime = GetTime();
}

PulsingOrb::~PulsingOrb() { }

void PulsingOrb::Print(std::ostream& os) const {
	os << "Pulsing Orb Renderer";
}




