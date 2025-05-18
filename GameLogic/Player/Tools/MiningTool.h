#pragma once
#include "../Inventory/IItem.h"

#include <string.h>

#include "../../../CoreGameLogic/Component.h"

class MiningTool : public IItem {
private:
	std::string name;
	ItemInfo info;

	float miningPower;
	int miningRadius;

public:
	MiningTool(std::string name, float miningPower, int miningRadius);

	void Equip() override;
	void UnEquip() override;
	const ItemInfo& GetInfo() const override;
	void DrawInfo(Rectangle& displayArea) override;
};
