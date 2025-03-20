#ifndef RENDERPASS_H
#define RENDERPASS_H
#include <functional>

#include "raylib.h"
#include "../EventSystem/ActionEvent.h"

class RenderPass {
	RenderTexture2D renderTexture;
	ActionEvent<RenderTexture2D&> renderFunctions;
public:
	RenderPass(int width, int height);
	RenderPass();

	void operator+=(function<void(RenderTexture2D&)> func);
	RenderTexture2D& operator()(RenderTexture2D& renderTexture);
};

#endif //RENDERPASS_H
