#include <external/glad.h>

#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"
#include "Libraries/stb_image.h"
#include "iostream"
#include "GameLogic/TerrainMinerTest.h"
#include "GameLogic/Map/MapFileReader.h"
#include "GameLogic/Map/MapGenerator.h"
#include "GameLogic/Map/Terrain.h"

//TODO: !!!!!!!IMPORTANT!!!!!!! Figure out RENDERING! (Render Scene vs Render into Textures)

// TODO: Implement a UI class
// TODO: Optimize square marching post processing shader
// TODO: Make square marching color depend on the actual weights
// TODO: Make mining outline like jj screenshot in square marching post processing shader
// TODO: Terrain collision
int main() {
	// auto map = MapFileReader::ReadMap("GeneratedMaps/Small.png");
	//
	// GameObject* terrain = new GameObject({0, 0});
	// terrain->AddComponent(new Terrain(map, 0.5, 1.0, 0, 8));
	//
	// GameObject* miner = new GameObject({0, 0});
	// miner->AddComponent(new TerrainMinerTest(1, 0.01));

	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Initialize(1000, 700, false);

	// GameObject* player = new GameObject({100, 100});
	// player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	// player->AddComponent(new PlayerMovement(600));
	return 0;
}
