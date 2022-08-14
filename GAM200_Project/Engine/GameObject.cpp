#include "../Engine/Collision.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Game.h"
#include "../Engine/ShowCollision.h"

#include "ShaderEffect.h"
#include "Texture.h"
#include "Sprite.h"
#include "GameObject.h"


GameObject::GameObject(): GameObject(math::vec2{ 0, 0 })
{}

GameObject::GameObject(math::vec2 position, math::vec2 scale, double rotation)
	: position(position), scale(scale), rotation(rotation), currState(nullptr), isDead(false), isStanding(false), shouldUpdateMatrix(true) {
	change_state(&stateNothing);
}
GameObject::~GameObject()
{
	clear_GO_component();
}

void GameObject::update(double dt)
{
	behaviorTimer -= dt;
	currState->update(this, dt);
	gameObjectComponentManager.update_all(dt);
	currState->test_for_exit(this);

	if (velocity.y < -1000 * dt)
		velocity.y = -1000 * dt;
}

void GameObject::draw(math::TransformMatrix viewportMatrix)
{
	if (get_GO_components<Sprite>() != nullptr) {
		get_GO_components<Sprite>()->draw(viewportMatrix * get_matrix());
	}
	if (get_GO_components<Texture>() != nullptr) {
		get_GO_components<Texture>()->draw(viewportMatrix * get_matrix());
	}

	if (get_GO_components<Sprite>() == nullptr && get_GO_components<Texture>() == nullptr) {
		if (get_GO_components<ShaderEffect>() != nullptr) {
			get_GO_components<ShaderEffect>()->draw(viewportMatrix * get_matrix());
		}
	}

	if (Engine::get_GE_component<Game>()->get_game_component<ShowCollision>()->is_enabled() &&
		get_GO_components<RectCollision>() != nullptr) {
		get_GO_components<RectCollision>()->draw(viewportMatrix);
	}
}

void GameObject::resolve_collision(GameObject*) {
	Engine::get_logger().log_error("This default resolve collision shouldn't be called!, GameObject.cpp, Line 49");
}

bool GameObject::can_collide_with(GOType) {
	return false;
}

GameObject::GOType GameObject::get_type() const
{
	return type;
}

void GameObject::set_type(GOType input)
{
	type = input;
}

std::string GameObject::get_name()
{
	return name;
}

void GameObject::set_name(std::string name_)
{
	name = name_;
}

math::vec2 GameObject::get_position()
{
	return position;
}

void GameObject::set_position(math::vec2 input)
{
	position = input;
}

void GameObject::update_position(math::vec2 input)
{
	position += input;
}

math::vec2 GameObject::get_velocity()
{
	return velocity;
}

void GameObject::set_velocity(math::vec2 input)
{
	velocity = input;
}

void GameObject::update_velocity(math::vec2 input)
{
	velocity += input;
}

math::vec2 GameObject::get_scale()
{
	return scale;
}

void GameObject::set_scale(math::vec2 input)
{
	scale = input;
}

double GameObject::get_rotation()
{
	return rotation;
}

void GameObject::set_rotation(double input)
{
	rotation = input;
}

bool GameObject::get_is_dead()
{
	return isDead;
}

void GameObject::set_is_dead(bool input)
{
	isDead = input;
}

bool GameObject::get_is_standing()
{
	return isStanding;
}

void GameObject::set_is_standing(bool input)
{
	isStanding = input;
}

bool GameObject::is_timer_done()
{
	return behaviorTimer <= 0;
}


void GameObject::set_timer(double input)
{
	behaviorTimer = input;
}

double GameObject::get_timer()
{
	return behaviorTimer;
}

void GameObject::State_Nothing::enter(GameObject*)
{}

void GameObject::State_Nothing::update(GameObject*, double)
{}

void GameObject::State_Nothing::test_for_exit(GameObject*)
{}

void GameObject::add_GO_component(Component* component)
{
	gameObjectComponentManager.add_component(component);
}

void GameObject::clear_GO_component()
{ 
	//Engine::get_logger().log_debug(get_name(), Logger::FontColor::Green);
	gameObjectComponentManager.clear_components(); 
}

math::TransformMatrix GameObject::get_matrix()
{
	return math::TranslateMatrix(position) * math::RotateMatrix(rotation) * math::ScaleMatrix(scale);
}

void GameObject::change_state(State* newState) {
	currState = newState;
	currState->enter(this);
}


