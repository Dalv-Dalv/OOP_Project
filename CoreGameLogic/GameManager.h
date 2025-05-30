#pragma once
#include "raylib.h"
#include "RenderPipeline.h"

class GameManager {
private:
	static GameManager* instance;
	GameManager() = default;

	int windowWidth, windowHeight;
	Rectangle screenRect;

	RenderPipeline* renderPipeline;
	shared_ptr<RenderPass> scenePass;

	static ActionEvent<> onGameClose;

	void InitializeRenderPipeline();

	void Awake();
	void Update(float deltaTime);
public:
	static GameManager* GetInstance();
	// static RenderTexture2D& GetActiveRenderTexture();
	// static void SetActiveRenderTexture(RenderTexture2D& renderTexture);

	static int GetWindowWidth();
	static int GetWindowHeight();

	static void SubscribeOnGameClose(const function<void()>& subscriber);

	static Rectangle GetScreenRect();
	static shared_ptr<RenderPass> GetScenePass();

	void Initialize(int windowWidth, int windowHeight, bool startInFullscreen, bool vsync = true);
	void StartGameLoop();

	static void Dispose();
};
