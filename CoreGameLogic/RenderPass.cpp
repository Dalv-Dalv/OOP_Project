#include "RenderPass.h"

#include "GameManager.h"

RenderPass::RenderPass(int width, int height) {
	renderTexture = LoadRenderTexture(width, height);
}
RenderPass::RenderPass() : RenderPass(GameManager::GetWindowWidth(), GameManager::GetWindowHeight()){}


void RenderPass::operator+=(function<void(RenderTexture2D&)> func) {
	renderFunctions += func;
}

RenderTexture2D& RenderPass::operator()(RenderTexture2D& renderTexture) {
	BeginTextureMode(this->renderTexture);

	renderFunctions(renderTexture);

	EndTextureMode();

	return this->renderTexture;
}
