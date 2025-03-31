#ifndef RENDERPIPELINE_H
#define RENDERPIPELINE_H
#include <queue>

#include "RenderPass.h"
#include "../EventSystem/ActionEvent.h"

class RenderPipeline {
private:
	static RenderPipeline* instance;
	RenderPipeline();

	RenderTexture2D screenRenderTexture;

	vector<shared_ptr<RenderPass>> renderPasses;
	ActionEvent<> onRenderScene;

public:
	static RenderPipeline* GetInstance();

	void Render();

	void AddRenderPass(const shared_ptr<RenderPass>& renderPass);
	void RemoveRenderPass(RenderPass* pass);

	static void DrawTextureFullScreen(const RenderTexture2D& texture);
};

#endif //RENDERPIPELINE_H
