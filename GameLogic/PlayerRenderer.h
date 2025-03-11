#ifndef PLAYERRENDERER_H
#define PLAYERRENDERER_H
#include "../CoreGameLogic/Component.h"

class PlayerRenderer : Component {
protected:
	void Update() override;
	void Start() override;
	using Component::Component;
};



#endif //PLAYERRENDERER_H
