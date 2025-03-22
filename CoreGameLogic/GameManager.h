#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "raylib.h"

class GameManager {
private:
	static GameManager* instancePtr;
	GameManager() {}

	int windowWidth, windowHeight;

	RenderTexture2D renderTexture;

	void Awake();
	void Update(float deltaTime);
	void OnGameClose();
public:
	GameManager(const GameManager& obj) = delete;

	static GameManager* GetInstance();
	static RenderTexture2D& GetActiveRenderTexture();
	static void SetActiveRenderTexture(RenderTexture2D& renderTexture);

	static int GetWindowWidth();
	static int GetWindowHeight();

	void Initialize(int windowWidth, int windowHeight, bool startInFullscreen);
};

#endif //GAMEMANAGER_H
