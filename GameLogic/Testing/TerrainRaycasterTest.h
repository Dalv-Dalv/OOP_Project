#pragma once
#include <ostream>
#include "../../CoreGameLogic/Component.h"
#include "../Map/Terrain.h"

class TerrainRaycasterTest : public Component {
private:
	Terrain* terrain = nullptr;

	RaycastHitInfo hitInfo;
protected:
	void Awake() override;
	void Update(float deltaTime) override;
	void Render();
public:
	TerrainRaycasterTest();
	~TerrainRaycasterTest();

	void Print(std::ostream &os) const override;
};
