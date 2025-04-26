#include "PlayerMovement.h"

#include <iostream>

#include <raylib.h>
#include "../CoreGameLogic/GameManager.h"
#include "../CoreGameLogic/GameObject.h"
#include "../Utilities/GameUtilities.h"
#include "../Utilities/Vector2Utils.h"
#include "Map/Terrain.h"
using namespace GameUtilities;

PlayerMovement* PlayerMovement::instance = nullptr;

void PlayerMovement::Awake() {
	prevInput = {0, 0};
	velocity = {0, 0};
	instance = this;
}

void PlayerMovement::Update(float deltaTime) {
	Vector2 input(0, 0);
	if(IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) input.y -= 1.0;
	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) input.x -= 1.0;
	if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) input.y += 1.0;
	if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) input.x += 1.0;
	input = V2Normalized(input);
	Vector2 finalInput = input * maxSpeed;

	velocity = lerp(velocity, finalInput, deltaTime * acceleration);

	float magnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	if(magnitude > 0.001) {
		velocity = velocity / magnitude;
		if(magnitude > maxSpeed) magnitude = maxSpeed;
		velocity = velocity * magnitude;

		Vector2 newPos = gameObject->position + velocity * deltaTime;
		auto collision = Terrain::CheckCollisions(newPos, colliderSize);
		if(collision.signedDist < colliderSize) {
			auto dir = collision.closestPoint - gameObject->position;
			dir = dir * fastInverseSqrt(dir.x*dir.x + dir.y*dir.y);
			dir *= dir.x * collision.normal.x + dir.y * collision.normal.y;

			velocity = Reflect(velocity, collision.normal) * 0.50;
			velocity -= collision.normal * 10.0;
			newPos = collision.closestPoint - dir * (colliderSize + 30.0);
			newPos += gameObject->position + velocity * deltaTime;
		}
		gameObject->position += velocity * deltaTime;
	}

	GameCamera::GetActiveCamera()->GetGameObject()->position = lerp(GameCamera::GetActiveCamera()->GetGameObject()->position, gameObject->position, deltaTime * 2.5);

	prevInput = finalInput;
}

void PlayerMovement::Print(std::ostream &os) const {
	os << "PlayerMovement";
}
