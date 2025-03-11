#include "Component.h"

#include "GameObject.h"

Component::Component() {
	components.insert(this);
}
Component::Component(GameObject* gameObject) : gameObject(gameObject) {
	components.insert(this);
}
Component::~Component() {
	components.erase(this);
}

void Component::SetGameObject(GameObject* gameObject) {
	this->gameObject = gameObject;
}

GameObject* Component::GetGameObject() const {
	return gameObject;
}


void Component::Destroy() {
	delete this;
}


