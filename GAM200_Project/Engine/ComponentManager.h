#pragma once
#include <vector>
#include "Component.h"

class ComponentManager
{
public:
	void add_component(Component*);
	template <typename T>
	T* get_component();

	template <typename T>
	void delete_component();
	void clear_components();
	size_t get_components_number();
	void update_all(double dt);
private:
	std::vector<Component*> components{};
};

template <typename T>
T* ComponentManager::get_component()
{
	for (auto& component : components) {
		T* comp = dynamic_cast<T*>(component);
		if (comp != nullptr) {
			return comp;
		}
	}
	return nullptr;
}

template <typename T>
void ComponentManager::delete_component()
{
	auto target = std::find_if(components.begin(), components.end(), [](Component* element) { return (dynamic_cast<T*>(element) != nullptr); });
	if (target != components.end()) {
		delete* target;
		components.erase(target);
	}
}
