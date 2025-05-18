#pragma once
#include "IItem.h"
#include "../../../CoreGameLogic/Component.h"

class PlayerInventory : public Component {
private:
	static PlayerInventory* activeInstance;

protected:
	void Awake() override;
	void Update(float deltaTime) override;

public:
	PlayerInventory();

	void Print(std::ostream& os) const override;

	static void GiveItem(IItem* item);
};
