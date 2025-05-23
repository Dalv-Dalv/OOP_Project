#pragma once
#include "raylib.h"
#include "../../CoreGameLogic/Component.h"

class PlayerRenderer : public Component {
private:
	float size;
	Color color;
protected:
	void Awake() override;
	void Update(float deltaTime) override;
	void Render() const;

public:
	PlayerRenderer(float size = 10.0f, Color color = BLUE);
	~PlayerRenderer();

	void Print(std::ostream &os) const override;
};
