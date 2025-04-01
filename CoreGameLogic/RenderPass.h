#pragma once
#include <functional>
#include <memory>

#include "GameCamera.h"
#include "raylib.h"
#include "../EventSystem/ActionEvent.h"

class RenderPass {
private:
	bool useCamera = false;
	int priority;

	RenderTexture2D renderTexture;
	ActionEvent<RenderTexture2D&> renderFunctions;

	friend class RenderPipeline;

public:
	RenderPass(int width, int height, int priority);
	~RenderPass();
	static shared_ptr<RenderPass> Create(int width, int height, int priority, bool useCamera = false);

	unsigned int AddFunction(const function<void(RenderTexture2D&)>& function);
	void RemoveFunction(unsigned int functionID);
	ActionEvent<RenderTexture2D&>& GetRenderFunctions();

	RenderTexture2D& Execute(RenderTexture2D& prev);
};
