#pragma once
#include <engine.h>
#include "engine/entities/bounding_box_bullet.h"

class player
{
public:
	player();
	~player();

	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	void turn(float angle);
	void update_camera(engine::perspective_camera& camera);

	void jump();

	void set_box(float width, float height, float depth, glm::vec3 position) { m_player_box.set_box(width, height, depth, position);}
	bounding_box& getBox() { return m_player_box; };

private:
	float m_speed{ 0.f };
	float m_timer;

	engine::ref< engine::game_object> m_object;

	bounding_box m_player_box;
};


