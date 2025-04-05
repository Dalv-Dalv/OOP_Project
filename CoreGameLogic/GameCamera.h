#pragma once
#include "Component.h"
#include <raylib.h>


class GameCamera : public Component {
private:
	Camera2D internalCamera;
	static GameCamera* instance;

protected:
	void Update(float deltaTime) override;
	void Awake() override;

public:
	GameCamera();

	Vector2 GetOffsetPos() const;
	float GetZoom();
	const Camera2D& GetCamera2D() const;
	Rectangle GetWorldSpaceScreenRect() const;

	static GameCamera* GetActiveCamera();
	static Vector2 ScreenToWorldCoords(Vector2 screenPos);


	void Print(std::ostream &os) const override;
};
