#include "../Engine/Input.h"
#include "../Engine/Engine.h"
#include "../Engine/Collision.h"
#include "../Engine/Game.h"
#include "../Engine/Audio.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/Camera.h"
#include "CollisionBlock.h"

#include <sstream>

CollisionBlock::CollisionBlock(math::vec2 pos, math::vec2 size) 
	: GameObject(pos)
{
	set_type(GOType::block);
	add_GO_component(new RectCollision({ { 0, 0 }, size }, this));
}

CollisionBlock::~CollisionBlock() {
	std::stringstream ss;
	ss << this;
	Engine::get_logger().log_debug(ss.str());
}

void CollisionBlock::resolve_collision(GameObject* objectPtr)
{
	auto gameObject = objectPtr;
	const auto thisWCR = get_GO_components<RectCollision>()->GetWorldCoorRect();
	switch (gameObject->get_type())
	{
		case GOType::player: [[fallthrough]];
		case GOType::boss: [[fallthrough]];
		case GOType::enemy:
		{
			bool collides[7];
			get_GO_components<RectCollision>()->will_collide(gameObject, collides);
			if (collides[6])
			{
				const auto directionVector = gameObject->get_velocity();
				const auto objPosition = gameObject->get_position();
				const auto objWCR = gameObject->get_GO_components<RectCollision>()->GetWorldCoorRect();
				const math::vec2 objLF = { objWCR.Left(), objWCR.Bottom() };
				const math::vec2 objRF = { objWCR.Right(), objWCR.Bottom() };
				const math::vec2 objLH = { objWCR.Left(), objWCR.Top() };
				const math::vec2 objRH = { objWCR.Right(), objWCR.Top() };
				if (collides[2])
				{
					gameObject->set_velocity({ 0.0, directionVector.y });
					gameObject->set_position({ thisWCR.Right() + objWCR.Size().x / 2., objPosition.y });
				}
				else if (collides[3])
				{
					gameObject->set_velocity({ 0.0, directionVector.y });
					gameObject->set_position({ thisWCR.Left() - objWCR.Size().x / 2. , objPosition.y });
				}
				else if (collides[4]) // �޹� (��, ���, ��)
				{
					if (objWCR.Bottom() >= thisWCR.Top()) // ��?
					{
						if (objWCR.Left() > thisWCR.Right()) // ����� �浹
						{
							math::vec2 positionCompareVector = math::vec2{ thisWCR.Right(), thisWCR.Top() } - objLF; // �÷��̾� �޹߿��� �� ���

							if (directionVector.Cross(positionCompareVector) <= 0) // ������
							{
								gameObject->set_velocity({ 0.0, directionVector.y });
								gameObject->set_position({ thisWCR.Right() + objWCR.Size().x / 2. , objPosition.y });

							}
							else // ��� - ��
							{
								gameObject->set_velocity({ directionVector.x, 0.0 });
								gameObject->set_position({ objPosition.x, thisWCR.Top() });
								gameObject->set_is_standing(true);
							}
						}
						else // ��
						{
							gameObject->set_velocity({ directionVector.x, 0.0 });
							gameObject->set_position({ objPosition.x, thisWCR.Top() });
							gameObject->set_is_standing(true);
						}
					}
					else // ��
					{
						gameObject->set_velocity({ 0.0, directionVector.y });
						gameObject->set_position({ thisWCR.Right() + objWCR.Size().x / 2., objPosition.y });
					}
				}
				else if (collides[5]) // ������ (��, �»�, ��)
				{
					if (objWCR.Bottom() >= thisWCR.Top()) // ��?
					{
						if (objWCR.Right() < thisWCR.Left()) // �»��� �浹
						{
							math::vec2 positionCompareVector = math::vec2{ thisWCR.Left(), thisWCR.Top() } - objRF; // ���� �߿��� �� �»�

							if (directionVector.Cross(positionCompareVector) >= 0) // ������
							{
								gameObject->set_velocity({ 0.0, directionVector.y });
								gameObject->set_position({ thisWCR.Left() - objWCR.Size().x / 2. , objPosition.y });// �»� - ��
							}
							else // �»� - ��
							{
								gameObject->set_velocity({ directionVector.x, 0.0 });
								gameObject->set_position({ objPosition.x, thisWCR.Top() });
								gameObject->set_is_standing(true);
							}
						}
						else // ��
						{
							gameObject->set_velocity({ directionVector.x, 0.0 });
							gameObject->set_position({ objPosition.x, thisWCR.Top() });
							gameObject->set_is_standing(true);
						}
					}
					else // ��
					{
						gameObject->set_velocity({ 0.0, directionVector.y });
						gameObject->set_position({ thisWCR.Left() - objWCR.Size().x / 2. , objPosition.y });
					}
				}
				else if (collides[0]) // �޸Ӹ� (��, ����, ��)
				{
					if (objWCR.Top() <= thisWCR.Bottom()) // ��?
					{
						if (objWCR.Left() >= thisWCR.Right()) // ������ �浹
						{
							math::vec2 positionCompareVector = math::vec2{ thisWCR.Right(), thisWCR.Bottom() } - objLH; // �޸Ӹ����� �� ����
							if (directionVector.Cross(positionCompareVector) >= 0) // ������
							{
								gameObject->set_velocity({ 0.0, directionVector.y });
								gameObject->set_position({ thisWCR.Right() + objWCR.Size().x / 2., objPosition.y }); // ���� - ��
							}
							else // ���� - ��
							{
								gameObject->set_velocity({ directionVector.x, 0.0 });
								gameObject->set_position({ objPosition.x, thisWCR.Bottom() - objWCR.Size().y });
							}
						}
						else // ��
						{
							gameObject->set_velocity({ directionVector.x, 0.0 });
							gameObject->set_position({ objPosition.x, thisWCR.Bottom() - objWCR.Size().y });
						}
					}
					else // ��
					{
						gameObject->set_velocity({ 0.0, directionVector.y });
						gameObject->set_position({ thisWCR.Right() + objWCR.Size().x / 2., objPosition.y });
					}
				}
				else if (collides[1]) // �����Ӹ� (��, ����, ��)
				{
					if (objWCR.Top() <= thisWCR.Bottom()) // ��?
					{
						if (objWCR.Right() <= thisWCR.Left()) // ������ �浹
						{
							math::vec2 positionCompareVector = math::vec2{ thisWCR.Left(), thisWCR.Bottom() } - objRH; // �÷��̾� �����Ӹ����� �� ����
							if (directionVector.Cross(positionCompareVector) <= 0) // ������
							{
								gameObject->set_velocity({ 0.0, directionVector.y });
								gameObject->set_position({ thisWCR.Left() - objWCR.Size().x / 2. , objPosition.y }); // ���� - ��
							}
							else // ���� - ��
							{
								gameObject->set_velocity({ directionVector.x, 0.0 });
								gameObject->set_position({ objPosition.x, thisWCR.Bottom() - objWCR.Size().y });
							}
						}
						else // ��
						{
							gameObject->set_velocity({ directionVector.x, 0.0 });
							gameObject->set_position({ objPosition.x, thisWCR.Bottom() - objWCR.Size().y });
						}
					}
					else // ��
					{
						gameObject->set_velocity({ 0.0, directionVector.y });
						gameObject->set_position({ thisWCR.Left() - objWCR.Size().x / 2. , objPosition.y });
					}
				}
			}
		}
		break;
		default:
			const auto collides = get_GO_components<RectCollision>()->does_collide(gameObject);
			if (collides)
			{
				gameObject->set_velocity({ gameObject->get_velocity().x, 0.0 });
				gameObject->set_position({ gameObject->get_position().x, thisWCR.Top() });
			}
			break;
	}

	

	// Engine::get_logger().log_debug("resolved from block");
}


void CollisionBlock::draw(math::TransformMatrix displayMt) {
	if (Engine::get_GE_component<Game>()->get_game_component<ShowCollision>()->is_enabled()) {
		get_GO_components<RectCollision>()->draw(displayMt);
		
	}
}

bool CollisionBlock::can_collide_with(GOType t) {
	return t != GOType::block &&t != GOType::hitbox;
}
