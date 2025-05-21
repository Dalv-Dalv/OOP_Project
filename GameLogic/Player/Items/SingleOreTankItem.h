#pragma once
#include "../../../CoreGameLogic/Component.h"
#include "../../Ores/IOreContainer.h"
#include "../../UI/UIOreMeter.h"
#include "../Inventory/IItem.h"

class SingleOreTankItem : public IItem, public Component, public IOreContainer {
private:
	float capacity;
	float oreType = -1;
	float oreFill = 0;


	Shader progressShader; int timeLoc, oreTypeLoc, fillAmountLoc, oreColorsLoc;
	Texture2D oreColors, whiteTex;

	ItemInfo info;

	bool isEquipped = false;

	UIOreMeter* uiOreMeter;

	void Awake() override;
	void Update(float deltaTime) override;

public:
	SingleOreTankItem(float capacity, const std::string& iconPath);
	~SingleOreTankItem();

	void AddOres(OreInfo& ores) override; // Ca tempalte  computeTime(Args...)
	void EmptyContents();

	void Equip() override;
	void UnEquip() override;
	const ItemInfo& GetInfo() const override;
	void DrawInfo(const Rectangle& displayArea) override;
	void AdaptDrawInfoRectangle(Rectangle& displayArea) override;

	void Print(std::ostream& os) const override;
	OreInfo GetContents() const override;
};
