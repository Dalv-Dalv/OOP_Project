#include <iostream>
#include <raylib.h>

int main() {
	InitWindow(600, 300, "Proiect POO");
	SetTargetFPS(144);
	SetConfigFlags(FLAG_VSYNC_HINT);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Pog", 20, 20, 30, ORANGE);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}