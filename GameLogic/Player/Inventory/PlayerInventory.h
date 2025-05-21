#pragma once
#include "IItem.h"
#include "../../../CoreGameLogic/Component.h"
#include "../../Ores/IOreContainer.h"
#include "../../Ores/OreInfo.h"

class PlayerInventory : public Component {
private:
	static PlayerInventory* activeInstance;
	IOreContainer* oreContainers[10] {nullptr};

	void HandleItemAdded(IItem* item, int index);
	void HandleItemRemoved(IItem* item, int index);
	void HandleItemSwapped(int i1, int i2);

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	PlayerInventory();

	void Print(std::ostream& os) const override;

	static void GiveItem(IItem* item);

	static void GiveOres(OreInfo& ores);
};
