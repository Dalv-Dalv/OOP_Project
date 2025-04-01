#pragma once
#include <queue>
#include <set>

#include "RenderPass.h"
#include "../EventSystem/ActionEvent.h"

class RenderPipeline {
private:
	static RenderPipeline* instance;
	RenderPipeline();

	RenderTexture2D screenRenderTexture;

	struct RenderPassComparator {
		bool operator()(const std::shared_ptr<RenderPass>& a, const std::shared_ptr<RenderPass>& b) const {
			return a->priority < b->priority;
		}
	};

	multiset<shared_ptr<RenderPass>, RenderPassComparator> renderPasses;
	ActionEvent<> onRenderScene;

public:
	static RenderPipeline* GetInstance();

	void Render();

	void AddRenderPass(const shared_ptr<RenderPass>& renderPass);
	void RemoveRenderPass(const shared_ptr<RenderPass>& pass);

	static void DrawTextureFullScreen(const RenderTexture2D& texture);
};