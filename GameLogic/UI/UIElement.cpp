#include "UIElement.h"

#include "UIManager.h"

int UIElement::maxZIndex = INT32_MIN;

UIElement::UIElement(bool stopsPropagation) : stopPropagation(stopsPropagation), zIndex(maxZIndex + 1), rect(0,0,0,0) {
	maxZIndex++;
	UIManager::Register(this);
};
UIElement::UIElement(int zIndex, Rectangle rect, bool stopsPropagation) : stopPropagation(stopsPropagation), zIndex(zIndex), rect(rect) {
	if(zIndex > maxZIndex) maxZIndex = zIndex;
	UIManager::Register(this);
};


UIElement::~UIElement() {
	UIManager::RemoveElement(this);
}

void UIElement::Destroy() {
	if(markedForDeletion) return;
	markedForDeletion = true;
	isActive = false;

	elementsMarkedForDeletion.push_back(this);
}
void UIElement::DeleteAllMarked() {
	for(auto elem : elementsMarkedForDeletion) {
		delete elem;
	}
	elementsMarkedForDeletion.clear();
}

bool UIElement::ContainsPoint(Vector2 point) const {
	return point.x >= rect.x && point.x <= rect.x + rect.width &&
		   point.y >= rect.y && point.y <= rect.y + rect.height;
}