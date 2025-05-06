#include "UIManager.h"

#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/RenderPipeline.h"

UIManager* UIManager::instance = nullptr;

void UIManager::Initialize() {
	if(instance != nullptr) return;
	instance = new UIManager();

	instance->renderPass = RenderPass::Create(100);
	instance->renderPass->AddFunction(Render);

	instance->whiteTex = LoadTexture("Textures/White.png");

	instance->oreMeterShader = LoadShader(0, "Shaders/oreMeterShader.frag");

	int screenSizeLoc = GetShaderLocation(instance->oreMeterShader, "screenSize");
	Vector2 screenSize(GameManager::GetWindowWidth(), GameManager::GetWindowHeight());
	SetShaderValue(instance->oreMeterShader, screenSizeLoc, &screenSize, SHADER_UNIFORM_VEC2);

	instance->timeLoc = GetShaderLocation(instance->oreMeterShader, "time");
	instance->meterSizeLoc = GetShaderLocation(instance->oreMeterShader, "meterSize");
}

void UIManager::Render(RenderTexture2D& prev) {
	RenderPipeline::DrawTextureFullScreen(prev);
	float fps = GetFPS();
	DrawText(TextFormat("%.1f", fps), 0, 0, 25, GREEN);

	float time = GetTime();
	Vector2 meterSize(100, 300);

	BeginShaderMode(instance->oreMeterShader);
		SetShaderValue(instance->oreMeterShader, instance->timeLoc, &time, SHADER_UNIFORM_FLOAT);
		SetShaderValue(instance->oreMeterShader, instance->meterSizeLoc, &meterSize, SHADER_UNIFORM_VEC2);
		DrawTexturePro(instance->whiteTex,
			Rectangle(0, 0, 1, 1),
			Rectangle(100, 400, meterSize.x, meterSize.y),
			Vector2(0, 0), 0, WHITE);
	EndShaderMode();
}

