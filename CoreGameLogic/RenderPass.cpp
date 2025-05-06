#include "RenderPass.h"

#include <iostream>

#include "GameManager.h"
#include "RenderPipeline.h"
#include <raylib.h>

RenderPass::RenderPass(int width, int height, int priority) : priority(priority) {
	renderTexture = LoadRenderTexture(width, height);
}
RenderPass::~RenderPass() {
	UnloadRenderTexture(renderTexture);
	renderFunctions.Clear();
}

shared_ptr<RenderPass> RenderPass::Create(int width, int height, int priority, bool useCamera) {
	auto ptr = make_shared<RenderPass>(width, height, priority);
	RenderPipeline::GetInstance()->AddRenderPass(ptr);
	ptr->useCamera = useCamera;
	return ptr;
}
shared_ptr<RenderPass> RenderPass::Create(int priority, bool useCamera) {
	auto ptr = make_shared<RenderPass>(GameManager::GetWindowWidth(), GameManager::GetWindowHeight(), priority);
	RenderPipeline::GetInstance()->AddRenderPass(ptr);
	ptr->useCamera = useCamera;
	return ptr;
}



unsigned int RenderPass::AddFunction(const function<void(RenderTexture2D&)>& function) {
	return renderFunctions.AddListener(function);
}
void RenderPass::RemoveFunction(unsigned int functionID) {
	renderFunctions.RemoveListener(functionID);
}

RenderTexture2D& RenderPass::Execute(RenderTexture2D& prev) {
	BeginTextureMode(this->renderTexture);
		if(useCamera) RenderPipeline::DrawTextureFullScreen(prev);
		if(useCamera) BeginMode2D(GameCamera::GetActiveCamera()->GetCamera2D());
		renderFunctions(prev);
		if(useCamera) EndMode2D();
	EndTextureMode();

	return this->renderTexture;
}

// Getters
ActionEvent<RenderTexture2D&>& RenderPass::GetRenderFunctions() { return renderFunctions; }
