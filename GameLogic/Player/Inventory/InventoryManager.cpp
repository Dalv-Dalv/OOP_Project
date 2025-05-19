#include "InventoryManager.h"

#include <iostream>
#include <stdexcept>

InventoryManager* InventoryManager::instance = nullptr;
ActionEvent<int> InventoryManager::onActiveSlotChanged;
ActionEvent<int> InventoryManager::onNrOfLockedSlotsChanged;
ActionEvent<int> InventoryManager::onItemRemoved;
ActionEvent<IItem*, int> InventoryManager::onItemAdded;
ActionEvent<int, int> InventoryManager::onItemsSwapped;

InventoryManager::InventoryManager() {
	slots = vector<InventorySlot>(10);
}
InventoryManager::~InventoryManager() {
	for (int i = 0; i < slots.size(); ++i) {
		delete slots[i].item;
	}
	slots.clear();
}


void InventoryManager::Initialize() {
	if(instance != nullptr) return;
	instance = new InventoryManager();

	onItemRemoved += [](int x) {
		if (x >= 0 && x < instance->slots.size()) {
			instance->slots[x].item = nullptr;
		}
	};

}

const IItem* InventoryManager::EquipItem(int index) {
	if(index < 0 || index > instance->slots.size()) return nullptr;
	if(instance->slots[index].locked) return nullptr;

	if(index == instance->activeSlotIndex) return instance->slots[index].item;

	if(instance->slots[instance->activeSlotIndex].item != nullptr) {
		instance->slots[instance->activeSlotIndex].item->UnEquip();
	}
	if(instance->slots[index].item != nullptr) {
		instance->slots[index].item->Equip();
	}

	instance->activeSlotIndex = index;
	onActiveSlotChanged.Invoke(index);

	return instance->slots[index].item;
}

void InventoryManager::SwapItems(int i1, int i2) {
	if(i1 < 0 || i1 > instance->slots.size() ||
	   i2 < 0 || i2 > instance->slots.size() || i1 == i2) return;
	if(instance->slots[i1].locked || instance->slots[i2].locked) return;

	if(i1 == instance->activeSlotIndex) {
		if(instance->slots[i1].item != nullptr) instance->slots[i1].item->UnEquip();
		if(instance->slots[i2].item != nullptr) instance->slots[i2].item->Equip();
	}else if(i2 == instance->activeSlotIndex) {
		if(instance->slots[i2].item != nullptr) instance->slots[i2].item->UnEquip();
		if(instance->slots[i1].item != nullptr) instance->slots[i1].item->Equip();
	}

	swap(instance->slots[i1], instance->slots[i2]);
	onItemsSwapped(i1, i2);
}

void InventoryManager::UnlockSlots(int count) {
	int target = min(int(instance->slots.size() - 1), instance->biggestUnlockedSlotIndex + count);
	for(int i = instance->biggestUnlockedSlotIndex + 1; i <= target; i++) {
		instance->slots[i].locked = false;
	}

	if(target == instance->biggestUnlockedSlotIndex) return;

	instance->biggestUnlockedSlotIndex = target;
	onNrOfLockedSlotsChanged.Invoke(target);
}
void InventoryManager::LockSlots(int count) {
	int target = max(0, instance->biggestUnlockedSlotIndex - count);
	for(int i = instance->biggestUnlockedSlotIndex; i > target; i--) {
		instance->slots[i].locked = true;
	}

	if(target == instance->biggestUnlockedSlotIndex) return;

	// If we locked a slot that we were using
	if(instance->activeSlotIndex < instance->biggestUnlockedSlotIndex && instance->activeSlotIndex > target) {
		EquipItem(0);
	}

	instance->biggestUnlockedSlotIndex = target;
	onNrOfLockedSlotsChanged.Invoke(target);
}

bool InventoryManager::IsFull() {
	for(int i = 0; i <= instance->biggestUnlockedSlotIndex; i++) {
		if(instance->slots[i].item == nullptr) return false;
	}
	return true;
}


const IItem* InventoryManager::GetActiveItem() {
	if(instance->slots[instance->activeSlotIndex].locked) return nullptr;
	return instance->slots[instance->activeSlotIndex].item;
}

const IItem* InventoryManager::GetItemAt(int index) {
	if(index < 0 || index > instance->slots.size()) return nullptr;
	if(instance->slots[index].locked) return nullptr;
	return instance->slots[index].item;
}

void InventoryManager::GiveItem(IItem* item) {
	if(IsFull()) {
		// QOL: Restructure Component and IItem such that they can be created and can exist without an attributed gameObject or slot
		throw new runtime_error("An item shouldnt be created if the inventory is full");
	}

	for(int i = 0; i <= instance->biggestUnlockedSlotIndex; i++) {
		if(instance->slots[i].item != nullptr) continue;

		instance->slots[i].item = item;
		onItemAdded.Invoke(item, i);

		if(instance->activeSlotIndex == i) {
			item->Equip();
		}
		return;
	}

	throw new runtime_error("Item could not be added for an unknown reason");
}




