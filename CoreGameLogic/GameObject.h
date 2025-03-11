#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <unordered_set>
#include "raylib.h"

class Component;

class GameObject {
private:
	std::unordered_set<Component*> components;
public:
	Vector2 position;
	GameObject();
	explicit GameObject(const Vector2& position);

	void AddComponent(Component* component);
};


#endif //GAMEOBJECT_H
