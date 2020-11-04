#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


player::player() : m_timer(0.0f), m_speed(1.0f)
{}

player::~player()
{}

void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, -1.f));
	m_object->set_position(glm::vec3(0.f, 0.5, 10.f));
	m_object->animated_mesh()->set_default_animation(1);
}

void player::on_update(const engine::timestep& time_step)
{
	m_object->set_position(m_object->position() += m_object->forward() * m_speed * (float)time_step);

	m_object->set_rotation_amount(atan2(m_object->forward().x, m_object->forward().z));

	if (engine::input::key_pressed(engine::key_codes::KEY_1)) {
		turn(1.0f * time_step);
	}
	else if (engine::input::key_pressed(engine::key_codes::KEY_2)) {
		turn(-1.0f * time_step);
	}

	//if (engine::input::key_pressed(engine::key_codes::KEY_SPACE)) {
	//	jump();
	//}

	if (m_timer > 0.0f)
	{
		m_timer -= (float)time_step;

		if (m_timer < 0.0f)
		{
			m_object->animated_mesh()->switch_root_movement(false);
			m_object->animated_mesh()->switch_animation(m_object->animated_mesh() -> default_animation());

			state = Idling;
 			if (last_state == Sprinting) {
				sprint(true);
			}
			else if (last_state == Walking) {
				sprint(false);
			}
		}
	}

	m_object->animated_mesh()->on_update(time_step * animation_speed);
}

void player::turn(float angle) {

	m_object->set_forward(glm::rotate(m_object->forward(), angle, glm::vec3(0.f, 1.f, 0.f)));
}

void player::update_first_person_camera(engine::perspective_camera& camera)
{
	auto [mouse_delta_x, mouse_delta_y] = engine::input::mouse_position();
	mouse_delta_x *= 0.1f;
	mouse_delta_y *= 0.1f;

	m_yaw += mouse_delta_x;
	m_pitch += mouse_delta_y;

	static const float pitch_limit = 89.0f;
	if (m_pitch > pitch_limit)
		m_pitch = pitch_limit;
	if (m_pitch < -pitch_limit)
		m_pitch = -pitch_limit;

	glm::vec3 front(0.f);
	float yaw_radians = glm::radians(m_yaw);
	float pitch_radians = glm::radians(m_pitch);
	front.x = cos(yaw_radians) * cos(pitch_radians);
	front.y = sin(pitch_radians);
	front.z = sin(yaw_radians) * cos(pitch_radians);
	

	float A = 0.95f;
	float B = 0.1f;

	glm::vec3 cam_pos = m_object->position() + glm::normalize(m_object->forward()) * B;;
	cam_pos.y += A;

	camera.set_view_matrix(cam_pos, front, front);
}

void player::update_third_person_camera(engine::perspective_camera& camera)
{
	float A = 2.f;
	float B = 3.f;
	float C = 6.f;

	glm::vec3 cam_pos = m_object->position() - glm::normalize(m_object->forward()) * B;
	cam_pos.y += A;

	glm::vec3 cam_look_at = m_object->position() + glm::normalize(m_object->forward() * C);
	cam_look_at.y = 1.5f;

	camera.set_view_matrix(cam_pos, cam_look_at);
}

void player::jump() {

	m_object->animated_mesh()->switch_root_movement(true);
	m_object->animated_mesh()->switch_animation(3);
	m_speed = 0.0f;
	animation_speed = 1.f;

	m_timer = (m_object->animated_mesh()->animations().at(3)->mDuration);

	last_state = state;
	state = Jumping;
}

void player::sprint(const bool& activateSprint) {

	if (activateSprint && state != Jumping) {
		state = Sprinting;
		m_speed = 2.0f;
		animation_speed = 1.5f;
	}
	else if(state != Jumping) {
		state = Walking;
		m_speed = 1.0f;
		animation_speed = 1.f;
	}
	else if (state == Jumping) {
		last_state = Walking;
	}

}
