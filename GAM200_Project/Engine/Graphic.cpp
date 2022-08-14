#include <array>
#include <sstream>
#include <iostream>
#include "Input.h"
#include "Graphic.h"
#include "Game.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Camera.h"

float IGL::Graphic::VIEWPORT_RATIO = 2.0f;

void IGL::Graphic::key_callback(GLFWwindow* , int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mod)
{
	switch (key)
	{
	case GLFW_KEY_ENTER:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Enter, action);
		break;
	case GLFW_KEY_ESCAPE:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Escape, action);
		break;
	case GLFW_KEY_SPACE:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Space, action);
		break;
	case GLFW_KEY_LEFT_SHIFT:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Shift, action);
		break;
	case GLFW_KEY_LEFT:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Left, action);
		break;
	case GLFW_KEY_RIGHT:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Right, action);
		break;
	case GLFW_KEY_UP:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Up, action);
		break;
	case GLFW_KEY_DOWN:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Down, action);
		break;
	case GLFW_KEY_GRAVE_ACCENT:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Tilde, action);
		break;
	case GLFW_KEY_LEFT_CONTROL:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Ctrl, action);
		break;
	case GLFW_KEY_TAB:
		Engine::get_GE_component<Input>()->set_key_down(InputKey::Keyboard::Tab, action);
		break;
	default:
		if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
			int offset = GLFW_KEY_A - static_cast<int>(InputKey::Keyboard::A);
			Engine::get_GE_component<Input>()->set_key_down(static_cast<InputKey::Keyboard>(key - offset), action);
		}
		else if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9) {
			int offset = GLFW_KEY_0 - static_cast<int>(InputKey::Keyboard::_0);
			Engine::get_GE_component<Input>()->set_key_down(static_cast<InputKey::Keyboard>(key - offset), action);
		}
		break;
	}


}

void IGL::Graphic::mousebutton_cb([[maybe_unused]] GLFWwindow* pwin, int button, int action, [[maybe_unused]] int mod)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		Engine::get_GE_component<Input>()->set_mouse_down(InputMouse::MouseButton::Left, action);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		Engine::get_GE_component<Input>()->set_mouse_down(InputMouse::MouseButton::Right, action);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		Engine::get_GE_component<Input>()->set_mouse_down(InputMouse::MouseButton::Center, action);
		break;
	}
}

IGL::Graphic::Graphic(const std::string& title)
{
	//네모 하나, 네모 각 꼭지점과 대응하는 픽셀
	create_window(1600, 900, title);
	glViewport(0, 0, windowWidth, windowHeight);
	set_background_color(0, 0, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

IGL::Graphic::Model IGL::Graphic::create_model() {
	Model model;

	std::array<VectexInfo, 4> vtx = { VectexInfo{ glm::vec2{0,0}, {0,0} },
												{ glm::vec2{1,0}, {1,0} },
												{ glm::vec2{1,1}, {1,1} },
												{ glm::vec2{0,1}, {0,1} } };
	const GLuint verts_size = 4;
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(VectexInfo) * verts_size, vtx.data(), GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &model.VAO_ID);
	glEnableVertexArrayAttrib(model.VAO_ID, 0);
	glVertexArrayVertexBuffer(model.VAO_ID, 3, vbo_hdl, 0, sizeof(VectexInfo));
	glVertexArrayAttribFormat(model.VAO_ID, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(model.VAO_ID, 0, 3);

	glEnableVertexArrayAttrib(model.VAO_ID, 1);
	glVertexArrayVertexBuffer(model.VAO_ID, 4, vbo_hdl, sizeof(glm::vec2), sizeof(VectexInfo));
	glVertexArrayAttribFormat(model.VAO_ID, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(model.VAO_ID, 1, 4);

	std::vector<GLushort> vtx_idx{ 2,1,3,0 };

	//EBO 
	GLuint ebo_hdl;
	unsigned int idx_size = static_cast<unsigned int>(vtx_idx.size());
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_size, reinterpret_cast<GLvoid*>(vtx_idx.data()), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(model.VAO_ID, ebo_hdl);
	glBindVertexArray(0);


	//model.primitive_type = GL_TRIANGLE_STRIP;
	//model.draw_cnt = idx_size;
	//model.primitive_cnt = idx_size;

	return model;
}


void IGL::Graphic::create_window(int w, int h, std::string t)
{
	windowWidth = w;
	windowHeight = h;
	if (!glfwInit()) {
		Engine::get_logger().log_error("GLFW init has failed - abort program!!!");
	}
	glfwSetErrorCallback(nullptr);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window_ptr = glfwCreateWindow(windowWidth, windowHeight, t.c_str(), NULL, NULL);
	if (!window_ptr) {
		Engine::get_logger().log_error("GLFW unable to create OpenGL context - abort program\n");
	}


	glfwMakeContextCurrent(window_ptr);
	glfwSetKeyCallback(window_ptr, Graphic::key_callback);
	glfwSetMouseButtonCallback(window_ptr, Graphic::mousebutton_cb);

	glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GLenum err = glewInit();
	if (GLEW_OK != err || !GLEW_VERSION_4_5) {
		Engine::get_logger().log_error("Driver doesn't support OpenGL 4.5 - abort program");
	}

	//glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void IGL::Graphic::set_background_color(int r, int g, int b, int a)
{
	float R = Helper::mapping(static_cast<float>(r), 0.f, 255.f, 0.f, 1.f);
	float G = Helper::mapping(static_cast<float>(g), 0.f, 255.f, 0.f, 1.f);
	float B = Helper::mapping(static_cast<float>(b), 0.f, 255.f, 0.f, 1.f);
	float A = Helper::mapping(static_cast<float>(a), 0.f, 255.f, 0.f, 1.f);

	glClearColor(R, G, B, A);
}

void IGL::Graphic::update(double)
{
	glfwPollEvents();
	Engine::get_GE_component<Input>()->set_prev_mouse_position(math::vec2{ mouseX, windowHeight - mouseY });
	glfwGetCursorPos(window_ptr, &mouseX, &mouseY);
	Engine::get_GE_component<Input>()->set_mouse_position(math::vec2{ mouseX, windowHeight - mouseY });
	glClear(GL_COLOR_BUFFER_BIT);
}

void IGL::Graphic::final() {
	glfwSwapBuffers(window_ptr);
}

math::vec2 IGL::Graphic::get_window_size() {
	return math::vec2{ windowWidth, windowHeight };
}

// scaled window value
math::vec2 IGL::Graphic::get_viewport_size() {
	return math::vec2{ windowWidth, windowHeight } / IGL::Graphic::VIEWPORT_RATIO;
}

