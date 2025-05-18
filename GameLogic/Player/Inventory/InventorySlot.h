#pragma once
#include <memory>

#include "IItem.h"

struct InventorySlot {
public:
	IItem* item = nullptr;

private:
	friend class InventoryManager;
	bool locked = false;
};
