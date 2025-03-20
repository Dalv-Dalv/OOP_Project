#ifndef PLAYERRENDERER_H
#define PLAYERRENDERER_H
#include "raylib.h"
#include "../CoreGameLogic/Component.h"

class PlayerRenderer : public Component {
private:
	float size;
	Color color;
protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	PlayerRenderer(float size = 10.0f, Color color = BLUE);
};

#endif //PLAYERRENDERER_H
