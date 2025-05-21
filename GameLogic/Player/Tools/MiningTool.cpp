#include "MiningTool.h"

#include <algorithm>
#include <format>
#include <iostream>

#include "raymath.h"
#include "../../../CoreGameLogic/GameObject.h"
#include "../../../CoreGameLogic/GameManager.h"
#include "../../../CoreGameLogic/InputManager.h"
#include "../../Map/Terrain.h"
#include "../Inventory/PlayerInventory.h"


MiningTool::MiningTool(const std::string& name, float miningPower, int miningRadius, Color rayColor, std::string iconPath)
	: name(name), miningPower(miningPower), miningRadius(miningRadius), rayColor(rayColor), IItem(iconPath) {
	info.itemName = name;
	info.itemDescription = std::format("Mining tool with a power of {} and a mining radius of {}", miningPower, miningRadius);
	info.usageDescription = "LMB";

	GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});
}



void MiningTool::Equip() {
	isEquipped = true;
}

void MiningTool::UnEquip() {
	isEquipped = false;
}

const ItemInfo& MiningTool::GetInfo() const {
	return info;
}
void MiningTool::DrawInfo(const Rectangle& displayArea) {}
void MiningTool::AdaptDrawInfoRectangle(Rectangle& displayArea) {}


void MiningTool::Awake() {}

void MiningTool::Print(std::ostream& os) const {
	os << "Mining tool: " << name;
}
void MiningTool::Update(float deltaTime) {
	if(!isEquipped) return;
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(InputManager::GetMousePosition());

	bool isMouseDown;
	if(InputManager::TryGetIsMouseDown(MOUSE_BUTTON_LEFT, isMouseDown) && isMouseDown) {
		isHoldingClick = true;

		Vector2 dir = worldPos - gameObject->position;
		float dist = Vector2Length(dir);
		dir = dir / dist;
		hitInfo = Terrain::GetActiveTerrain()->Raycast(gameObject->position, dir, dist);
		if(hitInfo.hasHit) {
			auto miningInfo = Terrain::GetActiveTerrain()->MineAt(hitInfo.hitPoint, miningRadius, miningPower, deltaTime);
			PlayerInventory::GiveOres(miningInfo);
		}
	}else {
		isHoldingClick = false;
	}
}

void MiningTool::Render() {
	if(!isEquipped) return;
	if(!isHoldingClick) return;
	DrawLineEx(gameObject->position, hitInfo.hitPoint, 4, rayColor);
	if(hitInfo.hasHit) {
		float r = sin(GetTime() * 20) * 9 + 15;
		DrawCircle(hitInfo.hitPoint.x, hitInfo.hitPoint.y, r, rayColor);
	}
	DrawCircle(hitInfo.hitPoint.x, hitInfo.hitPoint.y, sin(GetTime() * 15) * 4 + 8,  {unsigned char(clamp(rayColor.r + 50, 0, 255)), unsigned char(clamp(rayColor.g + 50, 0, 255)), unsigned char(clamp(rayColor.b + 50, 0, 255)), 255});
}





