#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"

int main() {
	GameObject* player = new GameObject({100, 100});
	player->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player->AddComponent(new PlayerMovement(500));

	GameObject* player1 = new GameObject({100, 400});
	player1->AddComponent(new PlayerRenderer(25.0f, BLUE));
	player1->AddComponent(new PlayerMovement(500));

	// TODO: Implement a UI class
	// TODO: Test shaders
	// TODO: Implement a post processing class
	// TODO: Terrain; ear clipping triangulation

	GameManager* gameManager = GameManager::GetInstance();

	int windowWidth = 1920, windowHeight = 1080;
	bool startInFullscreen = true;

	gameManager->Initialize(windowWidth, windowHeight, startInFullscreen);
	return 0;
}
