#pragma once
#include <unordered_set>
#include <vector>
#include "raylib.h"

class Component;

class GameObject {
private:
	std::vector<Component*> components;

	friend class GameManager;
	inline static std::unordered_set<GameObject*> gameObjects;
public:
	Vector2 position;
	GameObject();
	explicit GameObject(const Vector2& position);
	~GameObject();

	void AddComponent(Component* component);
};
