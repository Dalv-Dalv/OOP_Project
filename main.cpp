#include <set>
#include <external/glad.h>

#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"
#include "Libraries/stb_image.h"
#include "iostream"
#include "CoreGameLogic/RenderPipeline.h"
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

//TODO: CRITICALLLLLLLLLL:::::::::!!!!!!!!!!!!!!!!!!!!!!
//      CHECK CHATGPT  |  DEBUG flattenedValues AND regionValues USE VECTOR INSTEAD OF RAW POINTERS

int main() {
	GameObject* player = new GameObject({0, 0});
	player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player->AddComponent(new PlayerMovement(400));

	auto map = MapFileReader::ReadMap("GeneratedMaps/Small.png");

	GameObject* terrain = new GameObject({0, 0});
	terrain->AddComponent(new Terrain(map, 0.5, 0.8, 0, 16));

	GameObject* miner = new GameObject({0, 0});
	miner->AddComponent(new TerrainMinerTest(5, 3000.0));

	GameManager* gameManager = GameManager::GetInstance();
	gameManager->Initialize(1000, 800, false, true);

	gameManager->StartGameLoop();

	return 0;
}
