#include "OrbDeployer.h"

#include <iostream>

#include "PulsingOrb.h"
#include <raylib.h>
#include "../../CoreGameLogic/GameCamera.h"
#include "../../CoreGameLogic/GameObject.h"
#include "../../Utilities/Vector2Utils.h"

using namespace GameUtilities;

void OrbDeployer::Awake() {

}

void OrbDeployer::Update(float deltaTime) {
	if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
		ShootOrb();
	}
}


void OrbDeployer::Print(std::ostream& os) const {
	os << "OrbDeployer";
}

OrbDeployer::~OrbDeployer() {
	
}

void OrbDeployer::ShootOrb() {
	Vector2 dir = GameCamera::ScreenToWorldCoords(GetMousePosition()) - gameObject->position;
	dir *= fastInverseSqrt(dir.x*dir.x + dir.y*dir.y);
	auto* orb = new GameObject(gameObject->position);
	orb->AddComponent(new PulsingOrb(15, 0, dir * 400, GREEN));
}



