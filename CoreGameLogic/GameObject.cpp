#include "GameObject.h"

#include "Component.h"

GameObject::GameObject(const Vector2& position) : position(position) {}

void GameObject::AddComponent(Component* component) {
	component->SetGameObject(this);
	components.insert(component);
}
