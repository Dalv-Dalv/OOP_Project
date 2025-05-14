#pragma once
#include <memory>

#include "UIElement.h"
#include "../../CoreGameLogic/RenderPass.h"

using namespace std;

class UIManager {
private:
	static UIManager* instance;
	vector<UIElement*> elements;
	bool reorderingQueued = false;

	UIManager() = default;

	shared_ptr<RenderPass> renderPass;

	static void Render(RenderTexture2D& prev);
	void CheckEvents();
public:
	~UIManager();

	static void Initialize();

	static void RecalculateOrdering();

	static void Register(UIElement* element);
	static void RemoveElement(UIElement* element);

	static void Dispose();
};
