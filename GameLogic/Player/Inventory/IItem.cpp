#include "IItem.h"

#include "InventoryManager.h"

IItem::IItem(std::string iconPath) : slotIndex(-1) {
	icon = AssetManager::LoadTexture(iconPath);
	SetTextureWrap(icon, TEXTURE_WRAP_CLAMP);
}

IItem::~IItem() {
	InventoryManager::onItemRemoved.Invoke(slotIndex);
}