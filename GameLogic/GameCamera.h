#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include "raylib.h"
#include "../CoreGameLogic/Component.h"

class GameCamera : Component {
private:
	Camera2D camera;

	static GameCamera* mainCamera;

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	GameCamera();
	~GameCamera();

	Camera2D GetInternalCamera2D() const;
	void MakeMain();

	static GameCamera* GetMainCamera();
};

#endif //GAMECAMERA_H
