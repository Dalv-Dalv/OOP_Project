#pragma once
#include <string>

#include "ItemInfo.h"
#include "raylib.h"


class IItem {
private:
	int slotIndex;

public:
	IItem();
	virtual ~IItem();

	virtual void Equip() = 0;
	virtual void UnEquip() = 0;

	virtual const ItemInfo& GetInfo() const = 0;
	virtual void DrawInfo(Rectangle& displayArea) = 0;
};
