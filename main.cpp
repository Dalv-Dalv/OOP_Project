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
#include "GameLogic/Testing/TerrainMinerTest.h"
#include "GameLogic/Map/MapFileReader.h"
#include "GameLogic/Map/Terrain.h"
#include "GameLogic/Testing/OrbDeployer.h"
#include "GameLogic/Testing/TerrainRaycasterTest.h"
#include "GameLogic/UI/UIOreMeter.h"


/*----------------------------------------------------------------------
	CRITICAL: DEBUG WHY ORES ARE BROKEN VISUALLY
	WIP: Input manager and event passthrough
	WIP: UI
	WIP: Ores UI
	WIP: Ores
------------------------------------------------------------------------
	QOL: Make mining outline in square marching post processing shaders
	QOL: void(Args...) pt idamageable
------------------------------------------------------------------------
	OPTIMIZE: Square marching post processing shader
	OPTIMIZE: Collision detection
------------------------------------------------------------------------
	FINISHED: Terrain collision
	FINISHED: Handle object destruction
	FINISHED: Make square marching color depend on the actual weights
----------------------------------------------------------------------*/

int main() {
	auto* player = new GameObject({15901.1, 1202.34});
	player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player->AddComponent(new PlayerMovement(4000, 1.9, 22.0f));
	player->AddComponent(new OrbDeployer());
	player->AddComponent(new TerrainRaycasterTest());
	// player->AddComponent(new TerrainMinerTest(5, 0.1));

	TerrainData* map = nullptr;
	try {
		map = MapFileReader::ReadMap("GeneratedMaps/MapTest.png");
	}catch(const exception e) {
		cout << e.what() << endl;
		map = new TerrainData(0, 0);
	}

	auto* terrain = new GameObject({0, 0});
	terrain->AddComponent(new Terrain(map, 0.5, 1.5, TERRAIN_INTERPOLATION_AMOUNT, 128));

	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, USE_FULLSCREEN, USE_VSYNC);


	// UI Stuff
	new UIOreMeter(3);


	gameManager->StartGameLoop();

	return 0;
}