#include "SingleOreTankItem.h"

#include <iostream>

#include "../../../CoreGameLogic/InputManager.h"
#include "../../UI/UIOreMeter.h"
#include "../Inventory/InventoryManager.h"

SingleOreTankItem::SingleOreTankItem(float capacity, const std::string& iconPath) : IItem(iconPath), capacity(capacity) {
	info.itemName = "Single Ore Storage Tank";
	info.itemDescription = "Can hold only one type of ores at a time";
	info.usageDescription = "Press R while equipped to empty the tank";


	progressShader = AssetManager::LoadShader("Shaders/singleOreTankFillBar.frag");
	oreColors = AssetManager::LoadTexture("Textures/OreColors.png");
	whiteTex = AssetManager::LoadTexture("Textures/White.png");

	timeLoc = GetShaderLocation(progressShader, "time");
	oreTypeLoc = GetShaderLocation(progressShader, "oreType");
	fillAmountLoc = GetShaderLocation(progressShader, "fillAmount");
	oreColorsLoc = GetShaderLocation(progressShader, "oreColors");

	uiOreMeter = new UIOreMeter(this, capacity);
	uiOreMeter->ChangeScaledCapacity(capacity);
}
SingleOreTankItem::~SingleOreTankItem() {
	EmptyContents();
}


const ItemInfo& SingleOreTankItem::GetInfo() const {
	return info;
}

void SingleOreTankItem::Equip() {
	isEquipped = true;
}
void SingleOreTankItem::UnEquip() {
	isEquipped = false;
}

void SingleOreTankItem::Awake() {}
void SingleOreTankItem::Update(float deltaTime) {
	if(!isEquipped) return;

	if(IsKeyPressed(KEY_R)) {
		EmptyContents();
	}
}

void SingleOreTankItem::AddOres(OreInfo& ores) {
	for(int i = 0; oreType < 0 && i < 9; i++) {
		if(ores[i] <= 0) continue;
		oreType = i;
		uiOreMeter->ChangeOreType(oreType);
	}

	if(oreType < 0) return; // Its empty
	if(oreFill >= capacity) return;

	bool res = ores[oreType] > capacity - oreFill;

	if(ores[oreType] > capacity - oreFill) {
		float delta = capacity - oreFill;
		ores[oreType] -= delta;
		oreFill = capacity;

		uiOreMeter->UpdateFill(1);
	} else {
		oreFill += ores[oreType];
		ores[oreType] = 0;

		uiOreMeter->UpdateFill(oreFill / capacity);
	}
}
OreInfo SingleOreTankItem::GetContents() const {
	OreInfo contents;
	if(oreType < 0 || oreType > 9) return contents;
	contents[oreType] = oreFill;
	return contents;
}

void SingleOreTankItem::EmptyContents() {
	oreFill = 0;
	oreType = -1;
	uiOreMeter->ChangeOreType(-1);
	uiOreMeter->UpdateFill(0);
}






void SingleOreTankItem::AdaptDrawInfoRectangle(Rectangle& displayArea) {
	displayArea.height += 30 + 10; // For the progress bar and 10px padding
}

void SingleOreTankItem::DrawInfo(const Rectangle& displayArea) {
	Rectangle rect = displayArea;
	rect.y += rect.height - 40;
	rect.height = 30;

	rect.width = rect.width - 20;
	rect.x += 10;

	float time = GetTime();
	float fill = oreFill / capacity;

	BeginShaderMode(progressShader);
		SetShaderValue(progressShader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(progressShader, oreTypeLoc, &oreType, SHADER_UNIFORM_FLOAT);
		SetShaderValue(progressShader, fillAmountLoc, &fill, SHADER_UNIFORM_FLOAT);
		SetShaderValueTexture(progressShader, oreColorsLoc, oreColors);

		DrawTexturePro(whiteTex, Rectangle(0,0,1,1), rect, Vector2(0,0), 0, WHITE);
	EndShaderMode();
}

void SingleOreTankItem::Print(std::ostream& os) const {
	os << "SingleOreTank";
}






