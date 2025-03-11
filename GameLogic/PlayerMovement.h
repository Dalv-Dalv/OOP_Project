#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H
#include "raylib.h"
#include "../CoreGameLogic/Component.h"

class PlayerMovement : public Component {
private:
	float speed;
	Vector2 prevInput; // For input smoothing
protected:
	void Start() override;
	void Update() override;
public:
	PlayerMovement(float speed = 10) : speed(speed) {}
};

#endif //PLAYERMOVEMENT_H
