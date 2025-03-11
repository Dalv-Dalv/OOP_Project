#include "CoreGameLogic/GameManager.h"
#include "CoreGameLogic/GameObject.h"
#include "GameLogic/PlayerRenderer.h"

int main() {
	GameObject player({150, 150});
	player.AddComponent<PlayerRenderer>();

	GameManager* gameManager = GameManager::GetInstance();

	int windowWidth = 1920 * 0.8, windowHeight = 1080 * 0.8;
	bool startInFullscreen = false;

	gameManager->Initialize(windowWidth, windowHeight, startInFullscreen);
	return 0;
}
