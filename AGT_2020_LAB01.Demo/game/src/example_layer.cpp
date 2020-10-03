#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())
{
	// hide the mouse and lock it inside the window
	//engine::application::window().hide_mouse_cursor();

	float size = 1.2f;

	float radius = 2.f * size;
	uint32_t stacks = 10 * size;
	uint32_t slices = 20 * size;

	m_position = 0.0f;
	m_speed = 5.0f;

	engine::ref<engine::sphere> m_sphere_shape = engine::sphere::create(stacks, slices, radius);
	engine::game_object_properties sphere_props;
	sphere_props.meshes = { m_sphere_shape->mesh() };
	sphere_props.position = { 0.0f, 1.5f, -10.0f };

	m_sphere = engine::game_object::create(sphere_props);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	//Sleep(100);

	m_position += m_speed * time_step;
	if (m_position > 10.0f || m_position < -10) {
		m_position = -m_position;
	}
}

void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.0f, 0.0f, 0.0f, 1.0f });
	engine::render_command::clear();

	const auto colour_shader = engine::renderer::shaders_library()->get("colour");
	engine::renderer::begin_scene(m_3d_camera, colour_shader);

	glm::vec4 sphere_colour = glm::vec4(1.f, 0.f, 0.f, 1.f);
	glm::vec3 sphere_position = glm::vec3(m_position, 1.5f, -20.0f);

	create_sphere(colour_shader, sphere_colour, sphere_position); //red sphere along x-axis
	create_sphere(colour_shader, glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec3(0.f, m_position, -20.0f)); //green sphere along y-axis
	create_sphere(colour_shader, glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec3(0.f, 0.f, m_position-20.f)); //blue sphere along z

	//std::dynamic_pointer_cast<engine::gl_shader>(colour_shader)->set_uniform("a_color", sphere_colour);
	//glm::mat4 transform_1 = glm::translate(glm::mat4(1.0f), sphere_position);
	//engine::renderer::submit(colour_shader, m_sphere->meshes().at(0), transform_1);

	engine::renderer::end_scene();
}

void example_layer::create_sphere(const engine::ref<engine::shader>& colour_shader, glm::vec4& sphere_colour, glm::vec3& sphere_position) {

	std::dynamic_pointer_cast<engine::gl_shader>(colour_shader)->set_uniform("a_color", sphere_colour);
	glm::mat4 transform_1 = glm::translate(glm::mat4(1.0f), sphere_position);
	engine::renderer::submit(colour_shader, m_sphere->meshes().at(0), transform_1);
}

void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		if (e.key_code() == engine::key_codes::KEY_UP) {
			m_speed = m_speed + 2.0f; //10% faster
		}
		if (e.key_code() == engine::key_codes::KEY_DOWN) {
			m_speed = m_speed - 2.0f; //10% slower
		}
		if (e.key_code() == engine::key_codes::KEY_LEFT) {
			if (m_speed > 0) {
				m_speed = -m_speed; //changes direction to left
			}
		}
		if (e.key_code() == engine::key_codes::KEY_RIGHT) {
			if (m_speed < 0) {
				m_speed = -m_speed; //changes direction to right
			}
		}


		//speed cap
		if (m_speed > 100.0f) {
			m_speed = 100.0f;
		}
		else if (m_speed < -100.0f) {
			m_speed = -100.0f;
		}
		


	}
}
