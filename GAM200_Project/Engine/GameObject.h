#pragma once
#include "ComponentManager.h"
#include "Math.h"

class GameObject
{
	friend class Sprite;
public:
	enum class GOType
	{
		block,
		player,
		Researcher,
		enemy,
		boss,
		hitbox,
		dropItem,
		particle,
		count
	};

private: //Physics
	math::vec2 position;
	math::vec2 velocity;
	math::vec2 scale;
	double rotation;
	math::TransformMatrix objectMatrix;
protected: //GameObject information
	GOType type;
	std::string name;
protected: //Object Status
	bool isDead;
	bool isStanding;
	bool shouldUpdateMatrix;
	double behaviorTimer = 0;
public: // Basic Methods
	GameObject();
	GameObject(math::vec2 position, math::vec2 scale = { 1,1 }, double rotation = 0);
	virtual ~GameObject();
	virtual void update(double dt);
	virtual void draw(math::TransformMatrix viewportMatrix);
	virtual void resolve_collision(GameObject*);
	virtual bool can_collide_with(GOType t);
public: //Get-Set GameObject information
	GOType get_type() const;
	void set_type(GOType input);
	std::string get_name();
	void set_name(std::string name_);
public: //Get-Set Phsics 
	math::vec2 get_position();
	void set_position(math::vec2 input);
	void update_position(math::vec2 input);
	math::vec2 get_velocity();
	void set_velocity(math::vec2 input);
	void update_velocity(math::vec2 input);
	math::vec2 get_scale();
	void set_scale(math::vec2 input);
	double get_rotation();
	void set_rotation(double input);
	math::TransformMatrix get_matrix();
public: //Get-Set Status
	bool get_is_dead();
	void set_is_dead(bool input);
	bool get_is_standing();
	void set_is_standing(bool input);
	bool is_timer_done();
	void set_timer(double input);
	double get_timer();
	static constexpr math::vec2 DEFAULT_KNOCKBACK{ 4.0, 1.8 };
public: //Component Manage Methods
	template <typename T>
	T* get_GO_components() {
		return gameObjectComponentManager.get_component<T>();
	}


protected: //GameObject State
	class State {
	public:
		virtual void enter(GameObject* object) = 0;
		virtual void update(GameObject* object, double dt) = 0;
		virtual void test_for_exit(GameObject* object) = 0;
	protected:
		bool stateSwitch = false;
	};
	class State_Nothing : public State {
	public:
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	State_Nothing stateNothing;
	State* currState;
public:
	void change_state(State* newState);
	void add_GO_component(Component* component);
	template <typename T>
	void remove_GO_component()
	{
		gameObjectComponentManager.delete_component<T>();
	}
protected: //Component Manage Methods



	void clear_GO_component();
private: //Component Manager
	ComponentManager gameObjectComponentManager;
};
