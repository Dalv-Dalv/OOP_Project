#include "InputManager.h"
#include "raylib.h"

bool InputManager::captured = false;

bool InputManager::TryGetMousePosition(Vector2& out) {
	if(captured) return false;
	out = ::GetMousePosition();
	return true;
}
bool InputManager::TryGetIsMouseDown(int button, bool& out) {
	if(captured) return false;
	out = IsMouseButtonDown(button);
	return true;
}
bool InputManager::TryGetIsMouseUp(int button, bool& out) {
	if(captured) return false;
	out = IsMouseButtonUp(button);
	return true;
}

Vector2 InputManager::GetMousePosition() {
	return ::GetMousePosition();
}
bool InputManager::IsMouseDown(int button) {
	return IsMouseButtonDown(button);
}
bool InputManager::IsMouseUp(int button) {
	return IsMouseButtonUp(button);
}

void InputManager::CaptureMouse() {
	captured = true;
}
void InputManager::ResetCapture() {
	captured = false;
}






