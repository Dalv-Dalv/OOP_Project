#include "TerrainRaycasterTest.h"

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/GameObject.h"

void TerrainRaycasterTest::Awake() {
	terrain = Terrain::GetActiveTerrain();

	renderID = GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
		Render();
	});
}


void TerrainRaycasterTest::Update(float deltaTime) {
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());

	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 dir = worldPos - gameObject->position;
		float dist = Vector2Length(dir);
		dir = dir / dist;
		hitInfo = terrain->Raycast(gameObject->position, dir, dist);
		if(hitInfo.hasHit)
			terrain->MineAt(hitInfo.hitPoint, 3, 0.4, deltaTime);
	}

	//
	// if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
	// 	Vector2 dir = worldPos - gameObject->position;
	// 	float dist = Vector2Length(dir);
	// 	dir = dir / dist;
	// 	hitInfo = terrain->Raycast(gameObject->position, dir, dist);
	// }
}

void TerrainRaycasterTest::Render() {
	if(!IsKeyDown(KEY_R)) return;

	Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());

	int i = 20;
	float angleStep = 3.14159265f / (i - 1) * 0.5f;
	Vector2 pointingDir = worldPos - gameObject->position;
	float angle = atan2(pointingDir.y, pointingDir.x) - 3.14159265f * 0.25;
	float dist = 500;
	Vector2 prevHitPoint = {-1, -1};
	while(i > 0) {
		Vector2 dir = {cos(angle), sin(angle)};

		hitInfo = terrain->Raycast(gameObject->position, dir, dist);
		DrawLineEx(hitInfo.origin, hitInfo.hitPoint, 3, hitInfo.hasHit ? GREEN : RED);
		DrawCircle(hitInfo.hitPoint.x, hitInfo.hitPoint.y, 8, ORANGE);
		DrawLineEx(hitInfo.hitPoint, hitInfo.hitPoint + hitInfo.normal * 20, 1, RED);

		if(prevHitPoint.x > 0) {
			DrawLineEx(prevHitPoint, hitInfo.hitPoint, 5, BLUE);
		}

		prevHitPoint = hitInfo.hitPoint;

		i--;
		angle += angleStep;
	}


}

TerrainRaycasterTest::TerrainRaycasterTest() = default;
TerrainRaycasterTest::~TerrainRaycasterTest() {
	GameManager::GetScenePass()->RemoveFunction(renderID);
}

void TerrainRaycasterTest::Print(std::ostream &os) const {
	os << "TerrainRaycasterTest";
}