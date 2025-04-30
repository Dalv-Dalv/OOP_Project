#include "TerrainMinerTest.h"

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"

void TerrainMinerTest::Awake() {
	terrain = Terrain::GetActiveTerrain();
}


void TerrainMinerTest::Update(float deltaTime) {
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());

	// if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
	// 	terrain->MineAt(worldPos, miningRadius, miningPower, deltaTime);
	// }

	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 dir = worldPos - gameObject->position;
		float dist = Vector2Length(dir);
		dir = dir / dist;
		auto hitInfo = terrain->Raycast(gameObject->position, dir, dist);
		terrain->MineAt(hitInfo.hitPoint, miningRadius, miningPower, deltaTime);
	}
}


TerrainMinerTest::TerrainMinerTest() {}
TerrainMinerTest::TerrainMinerTest(int miningRadius, float miningPower)
	: miningRadius(miningRadius), miningPower(miningPower){}

void TerrainMinerTest::Print(std::ostream &os) const {
	os << "TerrainMinerTest";
}
