#pragma once
#include <vector>

#include "raylib.h"

class UIElement {
private:
	bool isActive = true;
	bool stopPropagation = true;

	int zIndex = 0;
	static int maxZIndex;

	inline static std::vector<UIElement*> elementsMarkedForDeletion;

	friend class UIManager;
	static void DeleteAllMarked();


protected:
	bool markedForDeletion = false;
	Rectangle rect;


public:
	UIElement(bool stopsPropagation = true);
	UIElement(int zIndex, Rectangle rect, bool stopsPropagation = true);
	virtual ~UIElement();

	virtual void Draw() = 0;

	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	virtual void OnHover() = 0;

	virtual bool ContainsPoint(Vector2 point) const;

	void Destroy();

	// Getters
	bool IsActive() const { return this->isActive; }
	bool DoesStopPropagation() const { return this->stopPropagation; }
	int GetZIndex() const { return this->zIndex; }
};
