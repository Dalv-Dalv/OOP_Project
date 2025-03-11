#ifndef GAMEELEMENT_H
#define GAMEELEMENT_H
#include <unordered_set>

class GameObject;
class Component {
private:
	friend class GameManager;
	static std::unordered_set<Component*> gameElements;

protected:
	virtual void Start() = 0;
	virtual void Update() = 0;

public:
	GameObject& gameObject;
	explicit Component(GameObject& gameObject);

	~Component();
	void Destroy();
};

#endif //GAMEELEMENT_H
