#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include <vector>

using namespace std;

template <typename... Args>
class ActionEvent {
private:
	vector<function<void(Args...)>> listeners;

public:
	void Invoke(Args... args);

	void operator+=(function<void(Args...)> func);
	void operator()(Args... args);
	void Clear();
};



// WHYYYY?
template <typename... Args>
void ActionEvent<Args...>::Invoke(Args... args) {
	for(auto& listener : listeners) {
		listener(args...);
	}
}


template <typename... Args>
void ActionEvent<Args...>::operator+=(function<void(Args...)> func) {
	listeners.push_back(func);
}
template <typename... Args>
void ActionEvent<Args...>::operator()(Args... args) {
	Invoke(args...);
}


template <typename... Args>
void ActionEvent<Args...>::Clear() {
	listeners.clear();
}


#endif //EVENT_H
