#pragma once
#include <string>

#include "ItemInfo.h"
#include "raylib.h"
#include "../../../CoreGameLogic/AssetManager.h"


class IItem {
private:
	int slotIndex;

protected:
	Texture2D icon;

public:
	explicit IItem(std::string iconPath);
	virtual ~IItem();

	virtual void Equip() = 0;
	virtual void UnEquip() = 0;

	virtual const ItemInfo& GetInfo() const = 0;
	virtual void DrawInfo(const Rectangle& displayArea) = 0;
	virtual void AdaptDrawInfoRectangle(Rectangle& displayArea) = 0;
	virtual const Texture2D& GetIcon() const {
		return icon;
	}
};
