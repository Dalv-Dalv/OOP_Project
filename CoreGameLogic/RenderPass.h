#pragma once
#include <functional>
#include <memory>

#include "GameCamera.h"
#include "raylib.h"
#include "../EventSystem/ActionEvent.h"

class RenderPass {
private:
	bool useCamera = false;

	RenderTexture2D renderTexture;
	ActionEvent<RenderTexture2D&> renderFunctions;

	friend class RenderPipeline;

public:
	RenderPass(int width, int height);
	RenderPass();
	~RenderPass();
	static shared_ptr<RenderPass> Create(int width, int height, bool useCamera = false);

	ActionEvent<RenderTexture2D&>& GetRenderFunctions();

	RenderTexture2D& Execute();
	RenderTexture2D& Execute(const RenderTexture2D& prev);
};
