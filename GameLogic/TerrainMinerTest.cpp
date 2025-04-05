#include "TerrainMinerTest.h"

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "../CoreGameLogic/GameManager.h"
#include "../CoreGameLogic/GameObject.h"

void TerrainMinerTest::Awake() {
	terrain = Terrain::GetActiveTerrain();
}


void TerrainMinerTest::Update(float deltaTime) {
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());

	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		terrain->MineAt(worldPos, miningRadius, miningPower, deltaTime);
	}

	if(IsKeyDown(KEY_E)) {
		surfaceLevel = Clamp(surfaceLevel + deltaTime * 0.5, 0, 1);
		terrain->UpdateSurfaceLevel(surfaceLevel);
	}else if(IsKeyDown(KEY_Q)) {
		surfaceLevel = Clamp(surfaceLevel - deltaTime * 0.5, 0, 1);
		terrain->UpdateSurfaceLevel(surfaceLevel);
	}
}


TerrainMinerTest::TerrainMinerTest() {}
TerrainMinerTest::TerrainMinerTest(int miningRadius, float miningPower)
	: miningRadius(miningRadius), miningPower(miningPower){}

void TerrainMinerTest::Print(std::ostream &os) const {
	os << "TerrainMinerTest";
}
