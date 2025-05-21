#include "IItem.h"

#include "InventoryManager.h"

IItem::IItem(const std::string& iconPath) : slotIndex(-1) {
	icon = AssetManager::LoadTexture(iconPath);
	SetTextureWrap(icon, TEXTURE_WRAP_CLAMP);
}

IItem::~IItem() {
	InventoryManager::onItemRemoved.Invoke(this, slotIndex);
}