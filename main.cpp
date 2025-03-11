#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerMovement.h"
#include "GameLogic/PlayerRenderer.h"

int main() {
	GameObject* player = new GameObject({100, 100});
	player->AddComponent(new PlayerRenderer(25.0f, GREEN));
	player->AddComponent(new PlayerMovement(500));


	GameManager* gameManager = GameManager::GetInstance();

	int windowWidth = 1920, windowHeight = 1080;
	bool startInFullscreen = true;

	gameManager->Initialize(windowWidth, windowHeight, startInFullscreen);
	return 0;
}
