#include "Component.h"

std::unordered_set<Component*> Component::gameElements;


Component::Component(GameObject& gameObject) : gameObject(gameObject) {}

Component::~Component() {
	gameElements.erase(this);
}

void Component::Destroy() {
	delete this;
}

