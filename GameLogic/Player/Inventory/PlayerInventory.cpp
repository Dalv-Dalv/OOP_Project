#include "PlayerInventory.h"

#include <iostream>

#include "InventoryManager.h"
#include "../../../CoreGameLogic/GameObject.h"
#include "../../../CoreGameLogic/InputManager.h"

PlayerInventory* PlayerInventory::activeInstance = nullptr;

PlayerInventory::PlayerInventory() {
	activeInstance = this;
}


void PlayerInventory::Awake() {}

void PlayerInventory::Update(float deltaTime) {
	int key = -1;
	if(IsKeyPressed(KEY_ONE))   key = 0;
	if(IsKeyPressed(KEY_TWO))   key = 1;
	if(IsKeyPressed(KEY_THREE)) key = 2;
	if(IsKeyPressed(KEY_FOUR))  key = 3;
	if(IsKeyPressed(KEY_FIVE))  key = 4;
	if(IsKeyPressed(KEY_SIX))   key = 5;
	if(IsKeyPressed(KEY_SEVEN)) key = 6;
	if(IsKeyPressed(KEY_EIGHT)) key = 7;
	if(IsKeyPressed(KEY_NINE))  key = 8;
	if(IsKeyPressed(KEY_ZERO))  key = 9;

	if(key == - 1) return;
	InventoryManager::EquipItem(key);
}

void PlayerInventory::Print(std::ostream& os) const {
	os << "PlayerInventory";
}

void PlayerInventory::GiveItem(IItem* item) {
	Component* compItem = dynamic_cast<Component*>(item);
	if(compItem != nullptr) {
		activeInstance->gameObject->AddComponent(compItem);
	}

	InventoryManager::GiveItem(item);
}


