#include "UIManager.h"
#include <algorithm>
#include <iostream>

#include "../../CoreGameLogic/GameManager.h"
#include "../../CoreGameLogic/InputManager.h"
#include "../../CoreGameLogic/RenderPipeline.h"

UIManager* UIManager::instance = nullptr;
UIManager::~UIManager() {
	for(UIElement* element : elements) {
		delete element;
	}
}


void UIManager::Initialize() {
	if(instance != nullptr) return;
	instance = new UIManager();

	instance->renderPass = RenderPass::Create(100);
	instance->renderPass->AddFunction(Render);
}

void UIManager::Render(RenderTexture2D& prev) {
	RenderPipeline::DrawTextureFullScreen(prev);
	float fps = GetFPS();
	DrawText(TextFormat("%.1f", fps), 0, 0, 25, GREEN);

	UIElement::DeleteAllMarked();

	instance->CheckEvents();
	for(UIElement* element : instance->elements) {
		if(!element->IsActive()) continue;
		element->Draw();
	}
}

void UIManager::CheckEvents() {
	if(reorderingQueued) {
		std::sort(elements.begin(), elements.end(), [](UIElement* a, UIElement* b) {
			return a->GetZIndex() < b->GetZIndex();
		});
		reorderingQueued = false;
	}

	Vector2 mousePos = InputManager::GetMousePosition();
	bool mousePressed = InputManager::IsMouseDown(MOUSE_LEFT_BUTTON);
	bool mouseUp = InputManager::IsMouseUp(MOUSE_LEFT_BUTTON);

	for(UIElement* element : elements) {
		if(!element->IsActive()) continue;
		if(!element->ContainsPoint(mousePos)) continue;

		element->OnHover();
		if(mousePressed) element->OnMouseDown();
		if(mouseUp) element->OnMouseUp();

		if(element->DoesStopPropagation()) {
			InputManager::CaptureMouse();
			break;
		}
	}
}

void UIManager::RecalculateOrdering() {
	instance->reorderingQueued = true;
}


void UIManager::Register(UIElement* element) {
	instance->elements.push_back(element);
	std::sort(instance->elements.begin(), instance->elements.end(), [](UIElement* a, UIElement* b) {
		return a->GetZIndex() < b->GetZIndex();
	});
}
void UIManager::RemoveElement(UIElement* element) {
	auto& elems = instance->elements;

	elems.erase(std::remove(elems.begin(), elems.end(), element), elems.end());
}



void UIManager::Dispose() {
	if(instance == nullptr) return;

	while (!instance->elements.empty()) {
		delete *instance->elements.begin();
	}
	instance->elements.clear();

	delete instance;
}



