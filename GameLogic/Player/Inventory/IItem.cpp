#include "IItem.h"

#include "InventoryManager.h"

IItem::IItem() : slotIndex(-1) {}

IItem::~IItem() {
	InventoryManager::onItemRemoved.Invoke(slotIndex);
}