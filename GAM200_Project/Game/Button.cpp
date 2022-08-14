#include "../Engine/Engine.h"
#include "../Engine/Texture.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Graphic.h"
#include "../Engine/Collision.h"
#include "../Engine/showCollision.h"
#include "../Engine/Input.h"
#include "../Engine/Game.h"
#include "Button.h"

Button::Button() : GameObject(), isClicked(false), isReleased(false) {
	set_type(GOType::count);
}

void Button::load(math::vec2 initPos, std::string buttonImage) {
	change_state(&stateNothing);
	set_position(initPos);
	if (texture == nullptr) {
		texture = Engine::get_GE_component<TextureManager>()->load(buttonImage);
		size = math::vec2{ texture->get_size().x / 2, texture->get_size().y };
	}

	if (get_GO_components<Collision>() == nullptr)
		add_GO_component(new RectCollision({ math::vec2{ 0.0, 0.0 }, size }, this));
}

void Button::unload() {
	clear_GO_component(); // only for removing RectCollision;
	Engine::get_GE_component<TextureManager>()->remove_texture(texture);
	texture = nullptr;
}

bool Button::is_button_pressed()
{
	return is_mouse_on_button() && Engine::get_GE_component<Input>()->is_mouse_pressed(InputMouse::MouseButton::Left);
}

bool Button::is_button_released()
{
	return is_mouse_on_button() && Engine::get_GE_component<Input>()->is_mouse_released(InputMouse::MouseButton::Left);
}

bool Button::is_mouse_on_button()
{
	if (get_GO_components<RectCollision>() == nullptr) return false;
	return get_GO_components<RectCollision>()->does_collide(Engine::get_GE_component<Input>()->get_mouse_position());
}

math::vec2 Button::get_button_size()
{
	return size;
}

void Button::update(double dt) {
	GameObject::update(dt);
}

void Button::draw(math::TransformMatrix) {
	texture->draw(get_matrix(), math::vec2{ is_mouse_on_button() ? size.x : 0.0, 0.0 }, size);

	if (Engine::get_GE_component<Game>()->get_game_component<ShowCollision>()->is_enabled() &&
		get_GO_components<RectCollision>() != nullptr) {
		get_GO_components<RectCollision>()->draw({});
	}
}

