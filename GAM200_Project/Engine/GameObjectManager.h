#pragma once
#include <list>
#include <map>
#include "Component.h"
#include "math.h"
enum class ObjType : int
{
	Block,
	Enemy,
	NPC,
	Player,
	E_Bullet,
	P_Bullet,
	Item,
	Particle
};

inline void operator++(ObjType& input)
{
	input = static_cast<ObjType>(static_cast<int>(input) + 1);
}

class GameObject;
class GameObjectManager : public Component
{ 
	//using GO = std::shared_ptr<GameObject>;
public:
	GameObjectManager() = default;
	~GameObjectManager() override;
	void update(double) override;
	void update_entire_position();
	void draw_all_game_object(math::TransformMatrix viewportMatrix);
	void add_game_object(ObjType, GameObject* gameObject);
	void clear_game_object();

	void collide_each_object();
	bool get_collidable();
	void set_collidable();
	void set_lock() { shouldUpdate = false; };
	void unlock() { shouldUpdate = true; }
public:
	const std::map<ObjType, std::list<GameObject*>>& objects() { return gameObjects; }
	const std::list<GameObject*>& objects(ObjType which) { return gameObjects[which]; }
	template<typename T>
	T* get_game_object(ObjType which);
	template<typename T>
	void delete_game_object(ObjType which);
private:
	std::map<ObjType, std::list<GameObject*>> gameObjects;
	bool noCollide = false;
	bool shouldUpdate = true;
};

template <typename T>
T* GameObjectManager::get_game_object(ObjType which)
{
	for (GameObject* gameObject : gameObjects[which]) {
		T* objPtr = dynamic_cast<T*>(gameObject);
		if (objPtr != nullptr) {
			return objPtr;
		}
	}
	return nullptr;
}

template <typename T>
void GameObjectManager::delete_game_object(ObjType which)
{
	if (gameObjects[which].empty()) return;
	auto target = std::find_if(gameObjects[which].begin(), gameObjects[which].end(), [](GameObject* element) { return (dynamic_cast<T*>(element) != nullptr); });
	delete* target;
	gameObjects[which].erase(target);
}
