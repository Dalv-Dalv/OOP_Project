#pragma once
#include "raylib.h"
#include "../../CoreGameLogic/Component.h"

class PulsingOrb : public Component {
private:
	unsigned int renderID = 0;
	float startTime = 0;
	float startV, sizeIncrease;
	Color color;

	float size = 0;

	Vector2 velocity = {0, 0};
protected:
	void Awake() override;
	void Update(float deltaTime) override;
	void Render() const;

public:
	PulsingOrb(float startV = 10, float sizeIncrease = 20, Vector2 startVelocity = {0, 0}, Color color = BLUE);
	~PulsingOrb();

	void Print(std::ostream &os) const override;
};
