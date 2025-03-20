#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H
#include <queue>

#include "RenderPass.h"
#include "../EventSystem/ActionEvent.h"

class RenderPipeline {
private:
	RenderTexture2D sceneTexture;

	vector<RenderPass> renderPasses;
	ActionEvent<> onRenderScene;
	ActionEvent<> onRenderUI;

public:
	RenderPipeline();

	void Render();

	void AddSceneRender(function<void()> sceneRender);
	void AddUIRender(function<void()> uiRender);
	void AddRenderPass(function<RenderTexture2D&(RenderTexture2D&)> renderPass);
	void AddRenderPass(RenderPass& renderPass);
};

#endif //RENDERPIPELINE_H
