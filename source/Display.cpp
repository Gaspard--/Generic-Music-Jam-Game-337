#include "Display.hpp"

GlfwInit::GlfwInit()
{
	glfwSetErrorCallback([](int, char const *str) {
				throw std::runtime_error(str);
			});
	if (!glfwInit()) {
		throw std::runtime_error("opengl: failed to init");
	}
}

GlfwInit::~GlfwInit()
{
	glfwTerminate();
}
