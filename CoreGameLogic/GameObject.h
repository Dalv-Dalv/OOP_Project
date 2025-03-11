#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <unordered_set>
#include "Component.h"
#include "raylib.h"
class GameObject {
private:
	std::unordered_set<Component*> components;
public:
	Vector2 position;
	GameObject();
	explicit GameObject(const Vector2& position);
	template <typename T>
	void AddComponent<T>();
};


#endif //GAMEOBJECT_H
