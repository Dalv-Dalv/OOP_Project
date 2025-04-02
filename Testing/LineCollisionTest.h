#pragma once
#include "../cmake-build-debug/_deps/raylib-src/src/raylib.h"
#include "../CoreGameLogic/Component.h"

class LineCollisionTest : public Component {

protected:
	void Update(float deltaTime) override;
public:
	// Check against a circle
	void CheckAgainst(Vector2 pos, float radius);

	void Print(std::ostream &os) const override;
};
