#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "raylib.h"

class GameManager {
private:
	static GameManager* instancePtr;
	GameManager() {}

	int windowWidth, windowHeight;

	void Awake();
	void Update();
public:
	GameManager(const GameManager& obj) = delete;

	static GameManager* GetInstance();

	static int GetWindowWidth();
	static int GetWindowHeight();

	void Initialize(int windowWidth, int windowHeight, bool startInFullscreen);
};

#endif //GAMEMANAGER_H
