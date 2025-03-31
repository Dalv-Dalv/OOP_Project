#include "RenderPass.h"

#include <iostream>

#include "GameManager.h"
#include "RenderPipeline.h"
#include <raylib.h>

RenderPass::RenderPass(int width, int height) {
	renderTexture = LoadRenderTexture(width, height);
}
RenderPass::RenderPass() : RenderPass(GameManager::GetWindowWidth(), GameManager::GetWindowHeight()){}
RenderPass::~RenderPass() {
	UnloadRenderTexture(renderTexture);
}

shared_ptr<RenderPass> RenderPass::Create(int width, int height, bool useCamera) {
	auto ptr = make_shared<RenderPass>(width, height);
	RenderPipeline::GetInstance()->AddRenderPass(ptr);
	ptr->useCamera = useCamera;
	return ptr;
}


ActionEvent<RenderTexture2D&>& RenderPass::GetRenderFunctions() {
	return renderFunctions;
}


RenderTexture2D& RenderPass::Execute() {
	BeginTextureMode(this->renderTexture);
		if(useCamera) BeginMode2D(GameCamera::GetActiveCamera()->GetCamera2D());
		renderFunctions(renderTexture);
		if(useCamera) EndMode2D();
	EndTextureMode();

	return this->renderTexture;
}

RenderTexture2D& RenderPass::Execute(const RenderTexture2D& prev) {
	BeginTextureMode(this->renderTexture);
		RenderPipeline::DrawTextureFullScreen(prev);
		if(useCamera) BeginMode2D(GameCamera::GetActiveCamera()->GetCamera2D());
		renderFunctions(renderTexture);
		if(useCamera) EndMode2D();
	EndTextureMode();

	return this->renderTexture;
}
