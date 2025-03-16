#include <external/glad.h>

#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"
#include "Libraries/stb_image.h"
#include "iostream"
#include "GameLogic/Map/MapFileReader.h"
#include "GameLogic/Map/MapGenerator.h"

int main() {
	auto map = MapFileReader::ReadMap("C:\\Dalv\\School\\University\\Classes\\Semestrul2\\POO\\ProiectPOO_Map\\GeneratedMaps\\Third.png");

	MapGenerator::GenerateMap(0.73, 0.5, map);

	// GameObject* player = new GameObject({100, 100});
	// player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	// player->AddComponent(new PlayerMovement(600));
	//
	// // TODO: Implement a UI class
	// // TODO: Implement a post processing class
	// // TODO: Terrain collision
	//
	// GameManager* gameManager = GameManager::GetInstance();
	//
	// int windowWidth = 1920, windowHeight = 1080;
	// bool startInFullscreen = true;
	//
	// gameManager->Initialize(windowWidth, windowHeight, startInFullscreen)
	return 0;
}
