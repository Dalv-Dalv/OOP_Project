#include "PlayerInventory.h"

#include <iostream>

#include "InventoryManager.h"
#include "../../../CoreGameLogic/GameObject.h"
#include "../../../CoreGameLogic/InputManager.h"
#include "../../Ores/OreInfo.h"

PlayerInventory* PlayerInventory::activeInstance = nullptr;

PlayerInventory::PlayerInventory() {
	activeInstance = this;

	InventoryManager::onItemAdded += [](IItem* item, int index) {
		activeInstance->HandleItemAdded(item, index);
	};

	InventoryManager::onItemRemoved += [](IItem* item, int index) {
		activeInstance->HandleItemRemoved(item, index);
	};
	InventoryManager::onItemsSwapped += [](int i1, int i2) {
		activeInstance->HandleItemSwapped(i1, i2);
	};
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
	auto compItem = dynamic_cast<Component*>(item);
	if(compItem != nullptr) {
		activeInstance->gameObject->AddComponent(compItem);
	}

	InventoryManager::GiveItem(item);
}

void PlayerInventory::GiveOres(OreInfo& ores) {
	for(int i = 0; i < 10; i++) {
		if(activeInstance->oreContainers[i] == nullptr) continue;

		activeInstance->oreContainers[i]->AddOres(ores);
	}
}


void PlayerInventory::HandleItemAdded(IItem* item, int index) {
	auto* oreContainer = dynamic_cast<IOreContainer*>(item);
	if(oreContainer == nullptr) return;

	oreContainers[index] = oreContainer;
}
void PlayerInventory::HandleItemRemoved(IItem* item, int index) {
	auto oreContainer = dynamic_cast<IOreContainer*>(item);
	if(oreContainer == nullptr) return;

	oreContainers[index] = nullptr;
}
void PlayerInventory::HandleItemSwapped(int i1, int i2) {
	swap(oreContainers[i1], oreContainers[i2]);
}

