#include "MiningTool.h"

#include <format>
#include <iostream>

MiningTool::MiningTool(std::string name, float miningPower, int miningRadius)
	: name(name), miningPower(miningPower), miningRadius(miningRadius) {
	info.itemName = name;
	info.itemDescription = std::format("Mining tool with a power of {} and a mining radius of {}", miningPower, miningRadius);
	info.usageDescription = "LMB";
}

void MiningTool::Equip() {
	std::cout << name << " Mining tool equipped\n";
}

void MiningTool::UnEquip() {
	std::cout << name << " Mining tool unequipped\n";
}

const ItemInfo& MiningTool::GetInfo() const {
	return info;
}

void MiningTool::DrawInfo(Rectangle& displayArea) {}


