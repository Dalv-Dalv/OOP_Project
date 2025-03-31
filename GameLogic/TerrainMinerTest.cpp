#include "TerrainMinerTest.h"

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "../CoreGameLogic/GameManager.h"

void TerrainMinerTest::Awake() {
	terrain = Terrain::GetActiveTerrain();
}


void TerrainMinerTest::Update(float deltaTime) {
	Vector2 mousePos = GetMousePosition();
	mousePos.y = GameManager::GetWindowHeight() - mousePos.y;

	if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		terrain->MineAt(mousePos, miningRadius, miningPower, deltaTime);
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
