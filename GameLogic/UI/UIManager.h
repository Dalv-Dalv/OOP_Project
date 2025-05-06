#pragma once
#include <memory>
#include "../../CoreGameLogic/RenderPass.h"

using namespace std;

class UIManager {
private:
	static UIManager* instance;
	UIManager() = default;

	shared_ptr<RenderPass> renderPass;
	Shader oreMeterShader; int timeLoc, meterSizeLoc;

	Texture2D whiteTex;

	static void Render(RenderTexture2D& prev);
public:
	static void Initialize();
};
