#ifndef PLAYERMOVEMENT_H
#define PLAYERMOVEMENT_H
#include "../CoreGameLogic/Component.h"

class PlayerMovement : public Component {
	void Start() override;
	void Update() override;
};

#endif //PLAYERMOVEMENT_H
