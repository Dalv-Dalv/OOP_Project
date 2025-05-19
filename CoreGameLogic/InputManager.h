#pragma once
#include "raylib.h"

class InputManager {
private:
	InputManager() = default;
	~InputManager() = default;
	static bool captured;
public:
	static bool TryGetMousePosition(Vector2& out);
	static bool TryGetIsMouseDown(int button, bool& out);
	static bool TryGetIsMouseUp(int button, bool& out);
	static bool TryGetIsMousePressed(int button, bool& out);

	static Vector2 GetMousePosition();
	static bool IsMouseDown(int button);
	static bool IsMouseUp(int button);
	static bool IsMousePressed(int button);

	static void CaptureMouse();
	static void ResetCapture();
};
