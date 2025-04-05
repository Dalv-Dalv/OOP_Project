#pragma once
#include <raylib.h>
#include "../CoreGameLogic/Component.h"

class PlayerMovement : public Component {
private:
	float speed;
	Vector2 prevInput; // For input smoothing
protected:
	void Awake() override;
	void Update(float deltaTime) override;
public:
	static PlayerMovement* instance; //TEMPORARY TODO:REMOVE LATER
	PlayerMovement(float speed = 10) : speed(speed) {}

	void Print(std::ostream &os) const override;
};
