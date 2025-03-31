#include "RenderPipeline.h"

#include "GameManager.h"

RenderPipeline* RenderPipeline::instance = nullptr;

RenderPipeline* RenderPipeline::GetInstance() {
	if(instance == nullptr) {
		instance = new RenderPipeline();
	}
	return instance;
}

RenderPipeline::RenderPipeline() {
	int width = GameManager::GetWindowWidth();
	int height = GameManager::GetWindowHeight();
	screenRenderTexture = LoadRenderTexture(width, height);
}


void RenderPipeline::Render() {
	RenderTexture2D& crnt = screenRenderTexture;
	BeginTextureMode(crnt);
		ClearBackground(MAGENTA);
	EndTextureMode();

	for(auto& pass : renderPasses) {
		crnt = pass->Execute(crnt);
	}

	int width = GameManager::GetWindowWidth();
	int height = GameManager::GetWindowHeight();
	Rectangle screenBounds = Rectangle(0, 0, width, height);
	BeginDrawing();
		DrawTextureRec(crnt.texture, screenBounds, {0, 0}, WHITE);

		float fps = GetFPS();
		DrawText(TextFormat("%.1f", fps), 0, 0, 25, GREEN);
	EndDrawing();
}

//TODO: Implement a data structure to keep render passes in order
void RenderPipeline::AddRenderPass(const shared_ptr<RenderPass>& renderPass) {
	renderPasses.push_back(renderPass);
}


void RenderPipeline::DrawTextureFullScreen(const RenderTexture2D& texture) {
	DrawTextureRec(texture.texture,GameManager::GetScreenRect(),{0, 0},WHITE);
}
