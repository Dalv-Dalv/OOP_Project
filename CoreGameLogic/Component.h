#pragma once
#include <ostream>
#include <unordered_set>

class GameObject;

class Component {
private:
	friend class GameManager;
	friend class GameObject;
	inline static std::unordered_set<Component*> components;
	void SetGameObject(GameObject* gameObject);

protected:
	GameObject* gameObject = nullptr;

	virtual void Awake() = 0;
	virtual void Update(float deltaTime) = 0;
public:
	Component();
	explicit Component(GameObject* gameObject);
	virtual ~Component();

	GameObject* GetGameObject() const;

	void Destroy();

	virtual void Print(std::ostream& os) const = 0;
	friend std::ostream& operator<<(std::ostream& os, const Component& component) {
		component.Print(os);
		return os;
	}
};
