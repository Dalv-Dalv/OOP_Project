#include "PlayerMovement.h"

#include "raylib.h"

void PlayerMovement::Start() {

}

void PlayerMovement::Update() {
	Vector2 input(0, 0);
	if(IsKeyDown(KEY_W)) input.y += 1.0;
	if(IsKeyDown(KEY_S)) input.y -= 1.0;
	if(IsKeyDown(KEY_A)) input.x -= 1.0;
	if(IsKeyDown(KEY_D)) input.x += 1.0;

}
