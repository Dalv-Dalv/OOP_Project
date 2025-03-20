#include "RenderPipeline.h"

#include "GameManager.h"
#include "../GameLogic/GameCamera.h"

RenderPipeline::RenderPipeline() {
	int width = GameManager::GetWindowWidth();
	int height = GameManager::GetWindowHeight();
	sceneTexture = LoadRenderTexture(width, height);
}


void RenderPipeline::Render() {
	GameCamera* camera = GameCamera::GetMainCamera();
	if(camera == nullptr) return;

	//Draw the scene
	BeginTextureMode(sceneTexture);
		BeginMode2D(camera->GetInternalCamera2D());
			onRenderScene();
		EndMode2D();
	EndTextureMode();

	RenderTexture2D& crnt = sceneTexture;
	for(auto& pass : renderPasses) {
		crnt = pass(crnt);
	}

	int width = GameManager::GetWindowWidth();
	int height = GameManager::GetWindowHeight();
	Rectangle screenBounds = Rectangle(0, 0, width, height);
	BeginDrawing();
		ClearBackground(MAGENTA);
		DrawTextureRec(crnt.texture, screenBounds, {0, 0}, WHITE);

		onRenderUI();
	EndDrawing();
}
