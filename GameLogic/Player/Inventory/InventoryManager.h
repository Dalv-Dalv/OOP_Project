#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "InventorySlot.h"
#include "../../../EventSystem/ActionEvent.h"

class InventoryManager {
private:
	friend class GameManager;
	static InventoryManager* instance;
	InventoryManager();

	std::vector<InventorySlot> slots;
	int biggestUnlockedSlotIndex = 0;

	int activeSlotIndex = 0;

public:
	~InventoryManager();

	static void Initialize();

	static ActionEvent<int> onActiveSlotChanged;
	static ActionEvent<int> onNrOfLockedSlotsChanged;
	static ActionEvent<int> onItemRemoved;
	static ActionEvent<IItem*, int> onItemAdded;
	static ActionEvent<int, int> onItemsSwapped;

	static const IItem* EquipItem(int index);
	static void SwapItems(int i1, int i2);

	// static void SaveToFile(string path);
	// static void LoadFromFile(string path);

	static void UnlockSlots(int count);
	static void LockSlots(int count);

	static bool IsFull();

	static const IItem* GetActiveItem();
	static const IItem* GetItemAt(int index);

	static void GiveItem(IItem* item);
};
