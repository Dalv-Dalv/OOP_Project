#pragma once
#include "../../CoreGameLogic/Component.h"

class OrbDeployer : public Component {
private:
	void ShootOrb();

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	~OrbDeployer() override;
	void Print(std::ostream& os) const override;
};
