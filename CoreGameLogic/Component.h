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

	virtual void Start() = 0;
	virtual void Update() = 0;
public:
	Component();
	explicit Component(GameObject* gameObject);
	~Component();

	GameObject* GetGameObject() const;

	void Destroy();
};

#endif //COMPONENT_H
