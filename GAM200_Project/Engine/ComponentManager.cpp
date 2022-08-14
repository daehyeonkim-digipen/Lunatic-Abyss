#include <typeinfo>
#include "Engine.h"
#include "ComponentManager.h"
#include "SceneManager.h"

void ComponentManager::add_component(Component* newComponent) {
	components.push_back(newComponent);
}

void ComponentManager::clear_components()
{
	for (Component* component : components) {
		Engine::get_logger().log_debug(typeid(*component).name());
		delete component;
	}
	components.clear();
}

size_t ComponentManager::get_components_number()
{
	return components.size();
}

void ComponentManager::update_all(double dt) {
	for (Component* component : components) {
		component->update(dt);
	}
}