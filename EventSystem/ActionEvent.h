#pragma once
#include <functional>
#include <vector>

using namespace std;

template <typename... Args>
class ActionEvent {
private:
	struct Listener {
		unsigned int id;
		function<void(Args...)> func;
	};
	vector<Listener> listeners;
	unsigned int nextID = 1;

public:
	void Invoke(Args... args) const;

	unsigned int AddListener(function<void(Args...)> func);
	void RemoveListener(unsigned int id);

	void operator+=(function<void(Args...)> func);
	void operator-=(unsigned int id);
	void operator()(Args... args) const;
	void Clear();
};


template <typename... Args>
void ActionEvent<Args...>::Invoke(Args... args) const {
	for(auto& listener : listeners) {
		listener.func(args...);
	}
}


template <typename... Args>
unsigned int ActionEvent<Args...>::AddListener(function<void(Args...)> func) {
	listeners.push_back({nextID, func});
	return nextID++;
}
template <typename... Args>
void ActionEvent<Args...>::RemoveListener(unsigned int id) {
	listeners.erase(remove_if(listeners.begin(), listeners.end(),
		[id](const Listener& listener) {
			return listener.id == id;
		}),listeners.end());
}



template <typename... Args>
void ActionEvent<Args...>::operator+=(function<void(Args...)> func) {
	AddListener(func);
}
template <typename... Args>
void ActionEvent<Args...>::operator-=(unsigned int id) {
	RemoveListener(id);
}
template <typename... Args>
void ActionEvent<Args...>::operator()(Args... args) const {
	Invoke(args...);
}


template <typename... Args>
void ActionEvent<Args...>::Clear() {
	listeners.clear();
}
