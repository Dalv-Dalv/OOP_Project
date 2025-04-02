#pragma once
#include "../CoreGameLogic/Component.h"
#include "Map/Terrain.h"

class TerrainMinerTest : public Component {
private:
	Terrain* terrain = nullptr;
	float surfaceLevel = 0.5;
	int miningRadius = 4;
	float miningPower = 30.0;
protected:
	void Awake() override;
	void Update(float deltaTime) override;
public:
	TerrainMinerTest();
	TerrainMinerTest(int miningRadius, float miningPower);
};
