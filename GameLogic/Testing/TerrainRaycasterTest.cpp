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
	// Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());
	//
	// if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
	// 	Vector2 dir = worldPos - gameObject->position;
	// 	float dist = Vector2Length(dir);
	// 	dir = dir / dist;
	// 	lastHitInfo = terrain->Raycast(gameObject->position, dir, dist);
	// }
}

void TerrainRaycasterTest::Render() {
	Vector2 worldPos = GameCamera::ScreenToWorldCoords(GetMousePosition());

	int i = 400;
	float angleStep = 3.14159265f / i * 2.0f;
	Vector2 pointingDir = worldPos - gameObject->position;
	float angle = atan2(pointingDir.y, pointingDir.x);
	angle = 0;
	float dist = 500;
	Vector2 prevHitPoint = {-1, -1};
	while(i > 0) {
		Vector2 dir = {cos(angle), sin(angle)};

		lastHitInfo = terrain->Raycast(gameObject->position, dir, dist);
		DrawLineEx(lastHitInfo.origin, lastHitInfo.hitPoint, 3, lastHitInfo.hasHit ? GREEN : RED);
		DrawCircle(lastHitInfo.hitPoint.x, lastHitInfo.hitPoint.y, 8, ORANGE);
		DrawLineEx(lastHitInfo.hitPoint, lastHitInfo.hitPoint + lastHitInfo.normal * 20, 1, RED);

		if(prevHitPoint.x > 0) {
			DrawLineEx(prevHitPoint, lastHitInfo.hitPoint, 5, BLUE);
		}

		prevHitPoint = lastHitInfo.hitPoint;

		i--;
		angle += angleStep;
	}

	if(IsKeyDown(KEY_R)) {
		Vector2 dir = worldPos - gameObject->position;
		float dist = Vector2Length(dir);
		dir = dir / dist;
		lastHitInfo = terrain->Raycast(gameObject->position, dir, dist);
		DrawCircle(lastHitInfo.hitPoint.x, lastHitInfo.hitPoint.y, 5, ORANGE);
		DrawLineEx(gameObject->position, lastHitInfo.hitPoint, 3, RED);
		terrain->MineAt(lastHitInfo.hitPoint, 3, 0.2, 1.0f/144);
	}
}

TerrainRaycasterTest::TerrainRaycasterTest() = default;
TerrainRaycasterTest::~TerrainRaycasterTest() {
	GameManager::GetScenePass()->RemoveFunction(renderID);
}

void TerrainRaycasterTest::Print(std::ostream &os) const {
	os << "TerrainRaycasterTest";
}