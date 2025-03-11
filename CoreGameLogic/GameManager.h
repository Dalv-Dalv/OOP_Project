#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class GameManager {
private:
	static GameManager* instancePtr;
	GameManager() {}

public:
	GameManager(const GameManager& obj) = delete;

	static GameManager* GetInstance();

	void Initialize(int windowWidth, int windowHeight, bool startInFullscreen);

	void Start();
	void Update();
};

#endif //GAMEMANAGER_H
