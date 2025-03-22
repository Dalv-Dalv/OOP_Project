#ifndef COMPONENT_H
#define COMPONENT_H
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
	virtual void OnGameClose() = 0;
public:
	Component();
	explicit Component(GameObject* gameObject);
	virtual ~Component();

	GameObject* GetGameObject() const;

	void Destroy();
};

#endif //COMPONENT_H
