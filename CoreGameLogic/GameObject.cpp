#include "GameObject.h"

#include "../GameLogic/PlayerRenderer.h"

GameObject::GameObject(const Vector2& position) : position(position) {}


template <typename T>
void GameObject::AddComponent<T>(){
	PlayerRenderer* player_renderer=new PlayerRenderer(*this);
	components.insert(player_renderer);
}
//
// void GameObject::AddComponent(Component* component) {
// 	component->gameObject = *this;
// 	components.insert(component);
// }

