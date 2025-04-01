#include "PlayerMovement.h"

#include <iostream>

#include "raylib.h"
#include "../CoreGameLogic/GameObject.h"
#include "../Utilities/GameUtilities.h"
#include "../Utilities/Vector2Utils.h"
using namespace GameUtilities;

void PlayerMovement::Awake() {
	prevInput = {0, 0};
}

void PlayerMovement::Update(float deltaTime) {
	Vector2 input(0, 0);
	if(IsKeyDown(KEY_W)) input.y += 1.0;
	if(IsKeyDown(KEY_A)) input.x -= 1.0;
	if(IsKeyDown(KEY_S)) input.y -= 1.0;
	if(IsKeyDown(KEY_D)) input.x += 1.0;
	input = V2Normalized(input);

	Vector2 finalInput = lerp(prevInput, input, deltaTime * 1.5);

	gameObject->position += finalInput * speed * deltaTime;

	prevInput = finalInput;
}
