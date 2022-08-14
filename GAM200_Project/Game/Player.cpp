#include "../Engine/Engine.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Game.h"
#include "../Engine/Audio.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Animation.h"
#include "../Engine/Camera.h"
#include "Gravity.h"
#include "Hitbox.h"
#include "Player.h"
#include "Particles.h"
#include "GOAttribute.h"
#include "../Engine/TextureManager.h"


Player::Player(math::vec2 pos)
	: GameObject(pos),
	moveLeftKey(GameKey::A), moveRightKey(GameKey::D), jumpKey(GameKey::Space), attackkey(GameMouse::Left),
	interactKey(GameKey::E), chargeKey(GameKey::Shift), guardKey(GameMouse::Right)
{
	//Setup GameObject information
	set_type(GOType::player);

	confusion = new GameObject({ 0, 0 });
	confusion->add_GO_component(new Sprite("assets/Sprite/Particles/confusion.spt", confusion));

	blind = new GameObject({ 0, 0 });
	blind->add_GO_component(Engine::get_GE_component<TextureManager>()->load("assets/Sprite/Player/hole.png"));

	superArmor = new GameObject({ 0, 0 });
	superArmor->add_GO_component(new Sprite("assets/Sprite/Particles/super.spt", superArmor));
	//confusion->set_scale({ 30, 30 });
	//Setup GO components
	add_GO_component(new Sprite("assets/Sprite/Player/player.spt", this));
	add_GO_component(new GOAttribute(100, 12, 100));
	get_GO_components<GOAttribute>()->set_ingame_status();
	//Setup Start State
	change_state(&stateFalling);
}

Player::~Player() {
	clear_GO_component();
}

//void Player::save_data()
//{
//	std::ofstream pData("assets/data/playerdata.txt");
//	pData << "HP";
//	pData << get_GO_components<GOAttribute>()->get_hp();
//	pData << "AD";
//	pData << get_GO_components<GOAttribute>()->get_ad();
//	pData.close();
//}

void Player::update(double dt) {
	//1. Current State Update
	//2. GO Components Update
	//3. Current State Exit
	if(playerStatus.freeze)
	{
		animationSpeedFactor = 0.2;
	} else
	{
		animationSpeedFactor = 0.3;
	}

	if (currState != &stateAttack && currState != &stateSuperAttack && currState != &stateCharging && currState != &stateDamaged)
	{
		patternTimer += dt;
	}
	else
	{
		patternTimer = 0;
	}
	playerStatus.superArmorTimer -= dt;
	playerStatus.confusionTimer -= dt;
	playerStatus.blindTimer -= dt; 
	playerStatus.freezeTimer -= dt;
	if(playerStatus.superArmorTimer < 0)
	{
		playerStatus.superArmor = false;
	}
	if (playerStatus.confusionTimer < 0)
	{
		playerStatus.confusion = false;
	}
	if (playerStatus.blindTimer < 0)
	{
		playerStatus.blind = false;
	}
	if (playerStatus.freezeTimer < 0)
	{
		playerStatus.freeze = false;
	}
	auto attribute = get_GO_components<GOAttribute>();

	if (patternTimer > 3)
	{
		attackPattern = 0;

		if (currState != &stateGuarding)
		{
			constexpr double gageReduce = 5;
			attribute->trans(gageReduce * dt);
		}
	}

	if (currState != &stateCharging && currState != &stateSuperAttack)
	{
		attribute->set_charge(0);
	}

	GameObject::update(dt);

	//Apply Gravity Always
	Gravity* gravity = Engine::get_GE_component<Game>()->get_GS_component<Gravity>();
	if (gravity != nullptr) {
		update_velocity(math::vec2{ 0.0, -gravity->get_value() * dt });
	}

	if (get_GO_components<GOAttribute>()->get_hp() <= 0 && currState != &stateDead) {
		change_state(&stateDead);
	}

	if (playerStatus.confusion)
	{
		confusion->set_position(get_position() + math::vec2{0, 30});
	}
	if (playerStatus.blind)
	{
		blind->set_position(get_position() + math::vec2{ 0, 25 } - blind->get_GO_components<Texture>()->get_size() / 2.);
	}

	if (playerStatus.superArmor)
	{
		superArmor->set_position(get_position() + math::vec2{ 0, 25 });
	}
}

void Player::update_moving(double dt) {
	if (!update_key_down(dt)) {
		update_fraction(dt);
	}
}

bool Player::update_key_down(double dt)
{
	const math::vec2 speed{ 100 * animationSpeedFactor , 0.0 };
	auto maxVelo = MOVEMENT_MAX_SPEED * dt;

	if (playerStatus.confusion && moveRightKey.is_key_down() || !playerStatus.confusion && moveLeftKey.is_key_down()) {
		set_scale({ -1, 1 });
		update_velocity(-math::vec2{ speed * dt });
		if (get_velocity().x <= -maxVelo) {
			set_velocity({ -maxVelo * animationSpeedFactor, get_velocity().y });
		}

	}
	else if (playerStatus.confusion && moveLeftKey.is_key_down() || !playerStatus.confusion && moveRightKey.is_key_down()) {
		set_scale({ 1, 1 });
		update_velocity((speed * dt));
		if (get_velocity().x >= maxVelo) {
			set_velocity({ maxVelo * animationSpeedFactor, get_velocity().y });
		}
	}
	else
	{
		return false;
	}
	return true;
}

void Player::update_fraction(double dt)
{
	math::vec2 speed;
	if (get_is_standing()) speed = { 100 * animationSpeedFactor , 0.0 };
	else speed = { 10 * animationSpeedFactor , 0.0 };


	math::vec2 friction{ 0.3, 1.0 };
	

	if (get_velocity().x > 0) {
		update_velocity(-math::vec2{ speed * friction * dt });
		if (get_velocity().x < 0)
			set_velocity({ 0.0, get_velocity().y });
	}
	if (get_velocity().x < 0) {
		update_velocity(math::vec2{ speed * friction * dt });
		if (get_velocity().x > 0)
			set_velocity({ 0.0, get_velocity().y });
	}
}

bool Player::is_player_bloking()
{
	return currState == &stateGuarding;
}

Player::PlayerStatus& Player::get_ps()
{
	return playerStatus;
}


void Player::draw(math::TransformMatrix viewportMatrix)
{
	if(get_GO_components<Sprite>())
	{
		if (playerStatus.freeze)
		{
			get_GO_components<Sprite>()->set_tint(0.5, 0.5, 2);
		}
		else
		{
			get_GO_components<Sprite>()->set_tint(1, 1, 1);
		}
	}
	GameObject::draw(viewportMatrix);
	if(playerStatus.confusion)
	{
		confusion->draw(viewportMatrix);
	}
	if(playerStatus.blind)
	{
		blind->draw(viewportMatrix);
	}
	if (playerStatus.superArmor)
	{
		superArmor->draw(viewportMatrix);
	}
}

void Player::resolve_collision(GameObject* object)
{
	switch (object->get_type())
	{
	case GOType::hitbox:
		Hitbox* box = static_cast<Hitbox*>(object);
		// Hitbox Can Give Damage Only One Time
		if (box->is_working() && box->owner->get_type() != GOType::player && box->does_hitted(this) == false) {
			auto attribute = get_GO_components<GOAttribute>();
			Engine::get_logger().log_debug(std::to_string(attribute->get_hp()));
			if (playerStatus.superArmor)
			{
				box->superArmor();
			}
			else if (currState == &stateGuarding && (box->is_defendable() || playerStatus.FTH1))
			{
				Engine::get_GE_component<GuardParticleEmitter>()->emit(1, get_position(), { 0, 0 }, { 0, 0 }, 0, get_scale());
				Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::GuardDamaged));
				if(attribute->get_gage() < 10)
				{
					attribute->set_df(0);
					behaviorTimer = 1 * (playerStatus.HLT1 ? 0.5 : 1);
					change_state(&stateDamaged);
				} else
				{
					box->no_knockback();
					attribute->update_gage(-10);
				}
			} 
			else
			{
				attribute->update_gage(-attribute->get_charge());
				Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Hurt));
				behaviorTimer = 2 * (playerStatus.HLT1 ? 0.5 : 1);
				change_state(&stateDamaged);
			}
		}
		break;
	}
}

bool Player::can_collide_with(GOType _type) {
	switch (_type)  // NOLINT(clang-diagnostic-switch-enum)
	{
	case GameObject::GOType::enemy: [[fallthrough]];
	case GameObject::GOType::hitbox: [[fallthrough]];
	case GameObject::GOType::dropItem:
		return true;
	default:
		return false;
	}
}

void Player::PlayerStateIdle::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->set_velocity({ 0.0, player->get_velocity().y });
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::idle));
}
void Player::PlayerStateIdle::update([[maybe_unused]] GameObject* object, [[maybe_unused]] double dt) {}
void Player::PlayerStateIdle::test_for_exit(GameObject* object) {

	if (Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->is_current_scene(Scenes::StatStage)) return;

	Player* player = static_cast<Player*>(object);
	if (player->moveLeftKey.is_key_down() || player->moveRightKey.is_key_down()) {
		player->change_state(&player->stateRunning);
	}
	if (player->playerStatus.confusion && player->guardKey.is_mouse_pressed() || !player->playerStatus.confusion && player->attackkey.is_mouse_pressed()) {
		player->change_state(&player->stateAttack);
	}
	if (player->jumpKey.is_key_pressed()) {
		player->change_state(&player->stateJump);
	}
	if (player->playerStatus.confusion && player->attackkey.is_mouse_pressed() || !player->playerStatus.confusion && player->guardKey.is_mouse_pressed())
	{
		player->change_state(&player->stateGuarding);
	}
}

void Player::PlayerStateRunning::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	stateSwitch = false;
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::running));
	if (player->playerStatus.freeze)
	{
		player->get_GO_components<Sprite>()->get_anim()->set_frame_speed(0.66);
	}
	else
	{
		player->get_GO_components<Sprite>()->get_anim()->set_frame_speed(1);
	}
}
void Player::PlayerStateRunning::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_moving(dt);

	auto FootstepAnimIndex = player->get_GO_components<Sprite>()->get_anim()->get_anim_index();

	if ((FootstepAnimIndex == 0 || FootstepAnimIndex == 3) && stateSwitch == false) {
		//Engine::get_GE_component<RunParticleEmitter>()->emit(1, player->get_position(), math::vec2{ 0.0,0.0 }, math::vec2{ 0.0,0.0 }, 0, math::vec2{ player->get_scale().x * 0.5,0.5 });
		if (rand() % 2 == 0) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Walk1));
			stateSwitch = true;
		}
		else {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Walk2));
			stateSwitch = true;
		}
	}
	else if (FootstepAnimIndex != 0 && FootstepAnimIndex != 3 && stateSwitch == true) {
		stateSwitch = false;
	}
}
void Player::PlayerStateRunning::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (math::is_equal(player->get_velocity().x, 0.)) {
		player->change_state(&player->stateIdle);
	}
	if (player->playerStatus.confusion && player->guardKey.is_mouse_pressed() || !player->playerStatus.confusion && player->attackkey.is_mouse_pressed()) {
		player->change_state(&player->stateAttack);
	}
	if (player->jumpKey.is_key_pressed()) {
		player->change_state(&player->stateJump);
	}
	if (player->get_velocity().y < 0) {
		player->change_state(&player->stateFalling);
	}
	if (player->playerStatus.confusion && player->attackkey.is_mouse_pressed() || !player->playerStatus.confusion && player->guardKey.is_mouse_pressed())
	{
		player->change_state(&player->stateGuarding);
	}
}

void Player::PlayerStateAttack::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	stateSwitch = false;
	if (player->chargeKey.is_key_down())
	{
		player->change_state(&player->stateCharging);
		return;
	}
	player->get_GO_components<Sprite>()->play_animation(player->attackPattern);
	math::vec2 mousePos = Engine::get_GE_component<Input>()->get_viewport_mouse_position() - (player->get_position() - Engine::get_GE_component<Game>()->get_game_component<Camera>()->get_position());

	if (mousePos.x > 0) {
		player->set_scale({ 1, 1 });
	}
	else {
		player->set_scale({ -1, 1 });
	}
	player->attackPattern = (player->attackPattern + 1) % 2;
	player->behaviorTimer = .4;
	if(player->playerStatus.STR2)
	{
		player->behaviorTimer = .3;
		player->get_GO_components<Sprite>()->get_anim()->set_frame_speed(1.33);
	}
}
void Player::PlayerStateAttack::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_fraction(dt);
	if (player->is_timer_done() && !stateSwitch) {
		player->set_position(player->get_position() + math::vec2{ 20., 0. } * player->get_scale());
		auto attribute = player->get_GO_components<GOAttribute>();
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Attack));
		const double dmgFactor = player->playerStatus.STR1 * (attribute->get_hp() / attribute->get_max_hp()) * 0.5;
		Hitbox* attackBox = new Hitbox(player, player->get_position(), math::rect2{ {0, 0}, {100, 100} }, attribute->get_ad() * (1. + dmgFactor), { 0, 0 });
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::P_Bullet, attackBox);
		Engine::get_GE_component<Game>()->get_game_component<Camera>()->shake({ 0, 13 }, 0.2, 4);
		stateSwitch = true;
	}

	if(player->attackkey.is_mouse_pressed())
	{
		repeat = true;
	}
}
void Player::PlayerStateAttack::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);

	if (repeat && player->attackPattern == 1 && player->get_GO_components<Sprite>()->get_anim()->get_anim_index() >= 4)
	{
		player->get_GO_components<Sprite>()->get_anim()->set_anim_index(4);
		if (!check && player->is_timer_done())
		{
			player->behaviorTimer = .4;
			check = true;
		}

		if (check && player->is_timer_done())
		{
			player->change_state(&player->stateAttack);
			repeat = false;
			check = false;
			return;
		}

	}
	if (player->get_GO_components<Sprite>() && player->get_GO_components<Sprite>()->is_animation_done())
	{
		player->change_state(&player->stateIdle);
	}

}

void Player::PlayerStateSuperAttack::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::Sa));
	player->set_timer(0.2);
	stateSwitch = false;
}
void Player::PlayerStateSuperAttack::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_fraction(dt);
	auto attribute = player->get_GO_components<GOAttribute>();
	if (player->is_timer_done() && !stateSwitch)
	{
		const auto factor = attribute->get_charge();
		attribute->update_charge(-factor);
		attribute->update_gage(-factor);

		player->set_position(player->get_position() + math::vec2{ 20., 0. } *player->get_scale());

		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::SuperAttack));
		const double dmgFactor = player->playerStatus.STR1 * (attribute->get_hp() / attribute->get_max_hp()) * 0.5;
		Hitbox* attackBox = new Hitbox(player, player->get_position(), math::rect2{ {0, 0}, {100, 100} }, attribute->get_ad() * pow(factor / 10. + 1, 1.5) * (1. + dmgFactor), math::vec2{ 0, 0 });
		Engine::get_GE_component<Game>()->get_game_component<Camera>()->shake({ 0, 15 }, 0.3, 0.02);
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::P_Bullet, attackBox);
		stateSwitch = true;
	}
}
void Player::PlayerStateSuperAttack::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);

	if (player->get_GO_components<Sprite>() && player->get_GO_components<Sprite>()->is_animation_done())
	{

		player->change_state(&player->stateIdle);
	}
}

void Player::PlayerStateJump::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::jumping));
	player->behaviorTimer = 0.1;
	stateSwitch = false;
}

void Player::PlayerStateJump::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_moving(dt);
	if (player->is_timer_done() && !stateSwitch) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Jump));
		player->set_is_standing(false);
		player->set_velocity(math::vec2{ player->get_velocity().x, 4. });
		stateSwitch = true;
	}
}

void Player::PlayerStateJump::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->get_velocity().y < 0) {
		player->change_state(&player->stateFalling);
	}

	if (player->attackkey.is_mouse_pressed()) {
		player->change_state(&player->stateAttack);
	}
}

void Player::PlayerStateFalling::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::falling));

	player->set_is_standing(false);
}

void Player::PlayerStateFalling::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_moving(dt);
}

void Player::PlayerStateFalling::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->get_is_standing()) {
		Engine::get_GE_component<StompParticleEmitter>()->emit(1, player->get_position() + math::vec2{ 0.0,20.0 }, math::vec2{ 0.0,0.0 }, math::vec2{ 0.0,0.0 }, 0, math::vec2{ 0.3,0.3 });
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Land));
		player->change_state(&player->stateIdle);
	}
	if (player->attackkey.is_mouse_pressed()) {
		player->change_state(&player->stateAttack);
	}
}

void Player::PlayerStateGuarding::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->playerStatus.HLT2 && player->chargeKey.is_key_down() && player->get_GO_components<GOAttribute>()->get_gage() > 30)
	{
		player->get_GO_components<GOAttribute>()->update_gage(-30);
		player->change_state(&player->stateSuperGuarding);
		return;
	}
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::guard));
	player->set_velocity({ 0,0 });
	player->set_timer(0.08);
	stateSwitch = false;
}


void Player::PlayerStateGuarding::update(GameObject* object, double dt)
{
	Player* player = static_cast<Player*>(object);
	player->update_fraction(dt);
	if (player->is_timer_done())
	{
		if(!stateSwitch)
		{
			stateSwitch = true;
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Guard));
			player->get_GO_components<GOAttribute>()->set_df(1);
		}
	}
}
void Player::PlayerStateGuarding::test_for_exit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);

	if (!player->guardKey.is_mouse_down())
	{
		player->get_GO_components<GOAttribute>()->set_df(0);
		player->change_state(&player->stateIdle);
	}
}

void Player::PlayerStateSuperGuarding::enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	auto& ps = player->playerStatus;
	ps.blind = false;
	ps.confusion = false;
	ps.freeze = false;
	ps.superArmor = true;
	ps.superArmorTimer = 5.;
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::SuperGuard));
	Engine::get_GE_component<SuperArmorParticleEmitter>()->emit(1, player->get_position()  + math::vec2{ 0., 50. }, { 0,0 }, { 0,0 }, 0, { 1,1 }, 0);
}

void Player::PlayerStateSuperGuarding::update(GameObject*, double)
{
}

void Player::PlayerStateSuperGuarding::test_for_exit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->change_state(&player->stateIdle);

}

void Player::PlayerStateCharging::enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->set_velocity({ 0,0 });
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Charge));
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::charge));
}

void Player::PlayerStateCharging::update(GameObject* object, double dt)
{
	Player* player = static_cast<Player*>(object);
	auto attribute = player->get_GO_components<GOAttribute>();
	constexpr double chargeSpeed = 10.;
	attribute->update_charge(dt * chargeSpeed);
}

void Player::PlayerStateCharging::test_for_exit(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	auto attribute = player->get_GO_components<GOAttribute>();

	if (player->jumpKey.is_key_down())
	{
		player->change_state(&player->stateJump);

	}
	else if (!player->attackkey.is_mouse_down() || !player->chargeKey.is_key_down() || attribute->get_charge() >= attribute->get_gage())
	{
		player->change_state(&player->stateSuperAttack);
	}
}


void Player::PlayerStateDamaged::enter(GameObject* object)
{
	Player* player = static_cast<Player*>(object);
	player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::damaged));
	player->set_is_standing(false);
	player->set_velocity(player->get_velocity() * 0.5);
}
void Player::PlayerStateDamaged::update(GameObject* object, double dt) {
	Player* player = static_cast<Player*>(object);
	player->update_fraction(dt);
}
void Player::PlayerStateDamaged::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);
	if (player->is_timer_done()) {
		player->change_state(&player->stateIdle);
	}
}

void Player::PlayerStateDead::enter(GameObject* object) {
	Player* player = static_cast<Player*>(object);

	if(player->get_GO_components<Sprite>())
	{
		if (player->playerStatus.FTH2 && !player->playerStatus.usedFTH2inThisStage)
		{
			player->playerStatus.usedFTH2inThisStage = true;
			player->change_state(&player->stateIdle);
			auto attri = player->get_GO_components<GOAttribute>();
			attri->set_hp(attri->get_max_hp() * 0.5);
			Engine::get_GE_component<ResurrectionParticleEmitter>()->emit(1, player->get_position() + math::vec2{ 0., 50. }, { 0,0 }, { 0,0 }, 0, { 1,1 }, 0);
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Resurrection));
			return;
		}
		player->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::dead));
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Dead));
	} else
	{
		player->set_timer(3);
	}
	player->set_velocity({ 0, 0 });
}
void Player::PlayerStateDead::update(GameObject* object, double)
{
	Player* player = static_cast<Player*>(object);
	player->set_velocity({ 0, 0 });
}
void Player::PlayerStateDead::test_for_exit(GameObject* object) {
	Player* player = static_cast<Player*>(object);

	if (player->get_GO_components<Sprite>() && player->get_GO_components<Sprite>() && player->get_GO_components<Sprite>()->is_animation_done() || !player->get_GO_components<Sprite>() && player->is_timer_done()) {
		player->set_is_dead(true);
	}
}


