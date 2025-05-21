#pragma once
#include "../Inventory/IItem.h"

#include <string.h>

#include "../../../CoreGameLogic/Component.h"
#include "../../Collisions/RaycastHitInfo.h"

class BallSpawnerTool : public IItem, public Component {
private:
	std::string name;
	ItemInfo info;

	bool isEquipped = false;
	bool isHoldingClick = false;
	RaycastHitInfo hitInfo;

	Color ballColor;

public:
	BallSpawnerTool(const std::string& name, Color ballColor, std::string iconPath);

	void Equip() override;
	void UnEquip() override;

	const ItemInfo& GetInfo() const override;
	void DrawInfo(const Rectangle& displayArea) override;
	void AdaptDrawInfoRectangle(Rectangle& displayArea) override;


protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	void Print(std::ostream& os) const override;
};
