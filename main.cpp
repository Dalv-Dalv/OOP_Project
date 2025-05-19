#include <set>

#include <external/glad.h>

#include "config.h"
#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/Player/PlayerMovement.h"
#include "GameLogic/Player/PlayerRenderer.h"
#include "Libraries/stb_image.h"
#include "iostream"
#include "CoreGameLogic/AssetManager.h"
#include "CoreGameLogic/RenderPipeline.h"
#include "GameLogic/Testing/TerrainMinerTest.h"
#include "GameLogic/Map/MapFileReader.h"
#include "GameLogic/Map/Terrain.h"
#include "GameLogic/Player/Inventory/InventoryManager.h"
#include "GameLogic/Player/Inventory/PlayerInventory.h"
#include "GameLogic/Player/Tools/BallSpawnerTool.h"
#include "GameLogic/Player/Tools/MiningTool.h"
#include "GameLogic/Testing/OrbDeployer.h"
#include "GameLogic/Testing/TerrainRaycasterTest.h"
#include "GameLogic/UI/UIOreMeter.h"
#include "GameLogic/UI/Inventory/UIInventory.h"


/*----------------------------------------------------------------------
	CRITICAL: MEMORY LEAK
	WIP: Ores UI
	WIP: Ores

	CRITICAL: BUG WITH INVENTORY, SWAP ITEMS REPEATEDLY
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
	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, USE_FULLSCREEN, USE_VSYNC);

	AssetManager::LoadShader("Shaders/inventorySlot.frag");
	AssetManager::LoadShader("Shaders/oreMeterShader.frag");
	AssetManager::LoadShader("Shaders/squareMarchingShader.frag");

	AssetManager::LoadTexture("Textures/White.png");
	AssetManager::LoadTexture("Textures/Transparent.png");
	AssetManager::LoadTexture("Textures/OreAtlas.png");
	AssetManager::LoadTexture("Textures/OreColors.png");
	AssetManager::LoadTexture("Textures/MapFGColors.png");
	AssetManager::LoadTexture("Textures/MapBGColors.png");
	AssetManager::LoadTexture("Textures/Items/DrillBig.png");
	AssetManager::LoadTexture("Textures/Items/Jackhammer.png");
	AssetManager::LoadTexture("Textures/Items/MiningWheel.png");

	// UI Stuff
	new UIOreMeter(1);
	new UIInventory(100, 10);

	auto* player = new GameObject({15901.1, 1202.34});
	player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player->AddComponent(new PlayerMovement(500, 1.9, 22.0f));
	player->AddComponent(new PlayerInventory());

	InventoryManager::UnlockSlots(3);

	PlayerInventory::GiveItem(new MiningTool("Fizz", 0.1f, 5, RED, "Textures/Items/DrillBig.png"));
	PlayerInventory::GiveItem(new MiningTool("Buzz", 0.5f, 2, BLUE, "Textures/Items/Jackhammer.png"));
	PlayerInventory::GiveItem(new BallSpawnerTool("Baller", GREEN, "Textures/Items/MiningWheel.png"));

	// player->AddComponent(new OrbDeployer());
	// player->AddComponent(new TerrainRaycasterTest());
	// player->AddComponent(new TerrainMinerTest(5, 0.1));

	TerrainData* map = nullptr;
	try {
		map = MapFileReader::ReadMap("GeneratedMaps/MapTest.png");
	}catch(const exception& e) {
		cout << e.what() << endl;
		map = new TerrainData(0, 0);
	}

	auto* terrain = new GameObject({0, 0});
	terrain->AddComponent(new Terrain(map, 0.5, 1.5, TERRAIN_INTERPOLATION_AMOUNT, 128));


	gameManager->StartGameLoop();

	return 0;
}