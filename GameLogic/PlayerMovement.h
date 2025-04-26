#pragma once
#include <raylib.h>
#include "../CoreGameLogic/Component.h"

class PlayerMovement : public Component {
private:
	float maxSpeed, acceleration, colliderSize;
	Vector2 prevInput; // For input smoothing
	Vector2 velocity;
protected:
	void Awake() override;
	void Update(float deltaTime) override;
public:
	static PlayerMovement* instance; //TEMPORARY TODO:REMOVE LATER
	PlayerMovement(float maxSpeed = 20, float acceleration = 10, float colliderSize = 25) : maxSpeed(maxSpeed), acceleration(acceleration), colliderSize(colliderSize), velocity(0, 0), prevInput(0, 0) {}

	void Print(std::ostream &os) const override;
};
