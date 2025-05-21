#include "BallSpawnerTool.h"
#include "MiningTool.h"

#include <format>
#include <iostream>

#include "../../../CoreGameLogic/GameObject.h"
#include "../../../CoreGameLogic/GameManager.h"
#include "../../../CoreGameLogic/InputManager.h"
#include "../../../Utilities/GameUtilities.h"
#include "../../../Utilities/Vector2Utils.h"
#include "../../Map/Terrain.h"
#include "../../Testing/PulsingOrb.h"

using namespace GameUtilities;

BallSpawnerTool::BallSpawnerTool(const std::string& name, Color ballColor, std::string iconPath)
	: name(name), ballColor(ballColor), IItem(iconPath) {
	info.itemName = name;
	info.itemDescription = std::format("Spawn balls");
	info.usageDescription = "LMB";
}



void BallSpawnerTool::Equip() {
	isEquipped = true;
}

void BallSpawnerTool::UnEquip() {
	isEquipped = false;
}

const ItemInfo& BallSpawnerTool::GetInfo() const {
	return info;
}
void BallSpawnerTool::DrawInfo(const Rectangle& displayArea) {}
void BallSpawnerTool::AdaptDrawInfoRectangle(Rectangle& displayArea) {}


void BallSpawnerTool::Awake() {}

void BallSpawnerTool::Print(std::ostream& os) const {
	os << "Ball spawning tool: " << name;
}
void BallSpawnerTool::Update(float deltaTime) {
	if(!isEquipped) return;
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(InputManager::GetMousePosition());

	bool isMouseDown;
	if(InputManager::TryGetIsMouseDown(MOUSE_BUTTON_LEFT, isMouseDown) && isMouseDown) {
		Vector2 dir = GameCamera::ScreenToWorldCoords(GetMousePosition()) - gameObject->position;
		dir *= fastInverseSqrt(dir.x*dir.x + dir.y*dir.y);
		auto* orb = new GameObject(gameObject->position);
		orb->AddComponent(new PulsingOrb(15, 0, dir * 400, ballColor));
	}
}





