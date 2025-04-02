#include "GameObject.h"

#include <iostream>

#include "Component.h"

GameObject::GameObject() : position{0, 0} {
	gameObjects.insert(this);
}
GameObject::GameObject(const Vector2& position) : position(position) {
	gameObjects.insert(this);
}
GameObject::~GameObject() {
	std::cout << "Destroying GameObject" << std::endl;
	while (!components.empty()) {
		std::cout << "Destroying Component " << *components.back();
		delete components.back();
		components.pop_back();
	}
	std::cout << std::endl;
	components.clear();
	gameObjects.erase(this);
}


void GameObject::AddComponent(Component* component) {
	component->SetGameObject(this);
	components.push_back(component);
}