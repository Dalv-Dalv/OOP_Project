#pragma once
#include <functional>
#include <optional>

using namespace std;

template <typename T, typename R>
class FunctionEvent {
private:
	vector<function<R(T)>> listeners;

public:
	optional<R> Invoke(T arg);

	void operator+=(function<R(T)> func);
	optional<R> operator()(T arg);
	void Clear();
};

template <typename T, typename R>
optional<R> FunctionEvent<T, R>::Invoke(T arg) {
	optional<R> result = nullopt;

	for(auto& listener : listeners) {
		result = listener(arg);
	}

	return result;
}


template <typename T, typename R>
void FunctionEvent<T, R>::operator+=(function<R(T)> func) {
	listeners.push_back(func);
}
template <typename T, typename R>
optional<R> FunctionEvent<T, R>::operator()(T arg) {
	return Invoke(arg);
}


template <typename T, typename R>
void FunctionEvent<T, R>::Clear() {
	listeners.clear();
}
