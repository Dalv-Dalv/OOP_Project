#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(const Vector2& position) : position(position) {}
GameObject::~GameObject() {
	// for(auto element : components) {
	// 	element->Destroy();
	// }
	components.clear();
}


void GameObject::AddComponent(Component* component) {
	component->SetGameObject(this);
	components.insert(component);
}

void GameObject::Destroy() {
	delete this;
}