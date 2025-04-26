#include <set>
#include <external/glad.h>

#include "config.h"
#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"
#include "Libraries/stb_image.h"
#include "iostream"
#include "CoreGameLogic/RenderPipeline.h"
#include "GameLogic/TerrainMinerTest.h"
#include "GameLogic/Map/MapFileReader.h"
#include "GameLogic/Map/Terrain.h"
#include "GameLogic/Testing/OrbDeployer.h"


/*----------------------------------------------------------------------
	WIP: Collision resolution
------------------------------------------------------------------------
	TODO: Make square marching color depend on the actual weights
------------------------------------------------------------------------
	QOL: Make mining outline in square marching post processing shaderdsssswa
------------------------------------------------------------------------
	OPTIMIZE: Square marching post processing shader
	OPTIMIZE: Collision detection
------------------------------------------------------------------------
	FINISHED: Terrain collision
	FINISHED: Handle object destruction
----------------------------------------------------------------------*/

int main() {
	auto* player = new GameObject({0, 0});
	player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player->AddComponent(new PlayerMovement(400, 0.9, 22.0f));
	player->AddComponent(new OrbDeployer());

	TerrainData* map = nullptr;
	try {
		map = MapFileReader::ReadMap("GeneratedMaps/Fourth.png");
	}catch(const exception e) {
		cout << e.what() << endl;
		map = new TerrainData(0, 0);
	}

	auto* terrain = new GameObject({0, 0});
	terrain->AddComponent(new Terrain(map, 0.5, 1.5, TERRAIN_INTERPOLATION_AMOUNT, 128));

	auto* miner = new GameObject({0, 0});
	miner->AddComponent(new TerrainMinerTest(5, 0.5));

	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, USE_FULLSCREEN, USE_VSYNC);

	gameManager->StartGameLoop();

	return 0;
}


/*
LineCollisionTest* LineCollisionTest::instance = nullptr;
CollisionInfo* LineCollisionTest::collisionInfo = new CollisionInfo(false, {0, 0});

LineCollisionTest::LineCollisionTest(Vector2 p1, Vector2 p2) : p1(p1), p2(p2), midP(0, 0) {}

void LineCollisionTest::Awake() {
    instance = this;
    GameManager::GetScenePass()->AddFunction([this](RenderTexture2D& prev) {
        DrawLineEx(p1, p2, 8, PURPLE);
        DrawLineEx(obP, midP, 4, DARKPURPLE);
    });
    GameManager::GetUIPass()->AddFunction([this](RenderTexture2D& prev) {
        DrawText(TextFormat("%.1f", dist), 0, 100, 25, PURPLE);
    });
}


void LineCollisionTest::Update(float deltaTime) {
    CheckAgainst(PlayerMovement::instance->GetGameObject()->position, 25.0f);
    p1.x = -100 - cos(GetTime()) * 50;
    p1.y = -100 - sin(GetTime()) * 50;
    p2.x = -100 + cos(GetTime()) * 50;
    p2.y = -100 + sin(GetTime()) * 50;
}


CollisionInfo LineCollisionTest::CheckAgainst(Vector2 pos, float radius) {
    Vector2 seg = p2 - p1;
    Vector2 toPoint = pos - p1;
    Vector2 normal = {0, 0};

    //https://stackoverflow.com/a/1501725
    const float l2 = seg.x * seg.x + seg.y * seg.y;
    if (l2 == 0.0) dist = 0;
    else {
        const float t = max(0.0f, min(1.0f, Vector2DotProduct(toPoint, seg) / l2));
        const Vector2 projection = p1 + seg * t;

        normal = pos - projection;

        midP = projection;
        dist = Vector2Distance(pos, projection);
    }

    obP = pos;

    // Signed distance
    float cross = seg.x * toPoint.y - seg.y * toPoint.x;
    dist *= (cross >= 0 ? 1 : -1);

    delete collisionInfo;
    collisionInfo = new CollisionInfo(abs(dist) < radius, {0, dist});
    return {abs(dist) < radius, normal};
}

void LineCollisionTest::Print(std::ostream &os) const {
    os << "LineCollisionTest";
}

*/