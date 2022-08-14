#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Engine.h"
#include "Math.h"


class Texture;
class ShaderEffect;

namespace IGL
{
	namespace Helper
	{
		template<typename T>
		T mapping(T value, T low1, T high1, T low2, T high2)
		{
			return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
		}

		template<typename T>
		T distanceSquare(T p1X, T p1Y, T p2X, T p2Y)
		{
			const T xDis = p1X - p2X;
			const T yDis = p1Y - p2Y;
			return xDis * xDis + yDis * yDis;
		}

		constexpr double distanceSquare(math::vec2 p1, math::vec2 p2)
		{
			const double xDis = p1.x - p2.x;
			const double yDis = p1.y - p2.y;
			return xDis * xDis + yDis * yDis;
		}
	}



	class Graphic : public Component
	{
	public:
		friend class Texture;
		friend class ShaderEffect;
	private:
		struct VectexInfo {
			glm::vec2 pos;
			glm::vec2 tex;
		};

		struct Model {
			unsigned int VAO_ID = 0; // id for vertex array object

			unsigned int primitive_type = GL_TRIANGLE_STRIP; // default
			unsigned int primitive_count = 2; // square with 2 triangles are default value, but is this used?
			unsigned int draw_count = 5; // number of tri_strip drawing lines. 3? 5?.. // 5 was fine to see
		};

		Model create_model();

		GLFWwindow* window_ptr;

	private:
		int windowWidth;
		int windowHeight;
		double mouseX;
		double mouseY;
		static void key_callback(GLFWwindow* , int , int , int , int );
		static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	public:
		Graphic(const std::string& title);
		void create_window(int w, int h, std::string t);
		math::vec2 get_window_size();
		math::vec2 get_viewport_size();
		void set_background_color(int r, int g, int b, int a);
		void update(double) override;
		void final();

		static float VIEWPORT_RATIO;
	};


}
