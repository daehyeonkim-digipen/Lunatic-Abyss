#include "GameObjectManager.h"
#include "Collision.h"
#include "Engine.h"
#include "GameObject.h"


GameObjectManager::~GameObjectManager()
{
	//Engine::get_logger().log_debug(">> " + (std::string)typeid(*this).name());
	for (auto lists : gameObjects)
	{
		for (GameObject* obj : lists.second)
		{
			delete obj;
		}
	}
	//Engine::get_logger().log_debug(">> ==========");
}

void GameObjectManager::update(double dt)
{
	if (!shouldUpdate) return;

	std::map<ObjType, GameObject*> destroyList;
	for (auto lists : gameObjects)
	{
		for (GameObject* obj : lists.second)
		{
			obj->update(dt);
			if (obj->get_is_dead() == true) {
				destroyList[lists.first] = obj;
			}
		}
	}
	for (auto object : destroyList)
	{
		gameObjects[object.first].remove(object.second);
		delete object.second;
	}
}
void GameObjectManager::update_entire_position()
{
	if (!shouldUpdate) return;

	for (auto lists : gameObjects)
	{
		for (GameObject* obj : lists.second)
		{
			obj->update_position(obj->get_velocity());
		}
	}
}
void GameObjectManager::draw_all_game_object(math::TransformMatrix viewportMatrix)
{
	for (auto lists : gameObjects)
	{
		for (GameObject* obj : lists.second)
		{
			obj->draw(viewportMatrix);
		}
	}
}

void GameObjectManager::add_game_object(ObjType which, GameObject* gameObject) {
	gameObjects[which].push_front(gameObject);
}

void GameObjectManager::clear_game_object()
{
	for (auto& lists : gameObjects)
	{
		if (lists.first == ObjType::Player) continue;
		for (GameObject* obj : lists.second)
		{
			delete obj;
		}
		lists.second.clear();
	}
}

void GameObjectManager::collide_each_object()
{
	if (!shouldUpdate) return;
	if (noCollide) return;
	// Block - Player Collision (n * 1) : swap order from first to here, does it matter?
	for (const auto block : gameObjects[ObjType::Block])
	{
		for (auto player : gameObjects[ObjType::Player])
		{
			if (player->get_GO_components<Collision>() != nullptr)
			{
				block->resolve_collision(player);
			}
		}
	}

	// Block - NPC Collision (n * 1)
	for (const auto block : gameObjects[ObjType::Block])
	{
		for (auto npc : gameObjects[ObjType::NPC])
		{
			if (npc->get_GO_components<Collision>() != nullptr)
			{
				block->resolve_collision(npc);
			}
		}
	}

	// Block - Enemy Collision (n * m)
	for (const auto block : gameObjects[ObjType::Block])
	{
		for (const auto enemy : gameObjects[ObjType::Enemy])
		{
			if (block->get_GO_components<RectCollision>() != nullptr && enemy->get_GO_components<RectCollision>() != nullptr)
			{
				block->resolve_collision(enemy);
			}
		}
	}

	// Block - Enemy Collision (n * m)
	for (const auto block : gameObjects[ObjType::Block])
	{
		for (auto item : gameObjects[ObjType::Item])
		{
			if (block->get_GO_components<Collision>() != nullptr && item->get_GO_components<Collision>() != nullptr)
			{
				block->resolve_collision(item);
			}
		}
	}

	// Bullet - Enemy Collision (n * m)
	for (const auto bullet : gameObjects[ObjType::P_Bullet])
	{
		for (auto enemy : gameObjects[ObjType::Enemy])
		{
			if (bullet->get_GO_components<Collision>() != nullptr && enemy->get_GO_components<Collision>() != nullptr && bullet->get_GO_components<RectCollision>()->does_collide(enemy))
			{
				bullet->resolve_collision(enemy);
				enemy->resolve_collision(bullet);
			}
		}
	}

	// Bullet - Player Collision (n * 1)
	for (const auto bullet : gameObjects[ObjType::E_Bullet])
	{
		for (auto player : gameObjects[ObjType::Player])
		{
			if (bullet->get_GO_components<Collision>() != nullptr && player->get_GO_components<Collision>() != nullptr && bullet->get_GO_components<RectCollision>()->does_collide(player))
			{
				bullet->resolve_collision(player);
			}
		}
	}

	// Item - Player Collision (n * 1)
	for (const auto item : gameObjects[ObjType::Item])
	{
		for (auto player : gameObjects[ObjType::Player])
		{
			if (item->get_GO_components<Collision>() != nullptr && player->get_GO_components<Collision>() != nullptr && item->get_GO_components<RectCollision>()->does_collide(player))
			{
				player->resolve_collision(item);
			}
		}
	}
}

bool GameObjectManager::get_collidable()
{
	return noCollide;
}

void GameObjectManager::set_collidable()
{
	noCollide = !noCollide;
}

