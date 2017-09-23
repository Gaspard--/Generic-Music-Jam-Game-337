#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include <fstream>
# include <sstream>
# include "GL/gl3w.h"
# include <GLFW/glfw3.h>
# include <memory>
# include "Vect.hpp"
# include "RenderContext.hpp"
# include "Bind.hpp"
# include "Rect.hpp"

# include <iostream> /* DEBUG */

static inline RenderContext contextFromFiles(std::string const &name)
{
	std::stringstream vert;
	std::stringstream frag;
	std::ifstream vertInput("shader/" + name + ".vert");
	std::ifstream fragInput("shader/" + name + ".frag");

	if (!fragInput || !vertInput) {
		throw std::runtime_error("Failed to open shader \"" + name +"\".");
	}
	vert << vertInput.rdbuf();
	frag << fragInput.rdbuf();
	return {Vao(), my_opengl::createProgram<2>(
			{(unsigned)GL_VERTEX_SHADER, (unsigned)GL_FRAGMENT_SHADER},
			{vert.str(), frag.str()}
		)};
}

struct GlfwInit
{
	GlfwInit();
	~GlfwInit();
};

template<size_t width, size_t height>
class Display : GlfwInit
{
	using window_ptr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;
	window_ptr window;

	RenderContext rectContext;
	glBuffer rectBuffer;

	Vect<2, float> dim;
	Vect<2, float> size;

public:
	Display(void)
		: window([this]() {
				window_ptr window(glfwCreateWindow(width, height, "Wow such fenêtre. Waouh. Much masterisation d'opengl, lel", nullptr, nullptr), &glfwDestroyWindow);

				if (!window) {
					throw std::runtime_error("opengl: failed to open window");
				}
				glfwMakeContextCurrent(window.get());
				glfwSwapInterval(1);
				if (gl3wInit()) {
					throw std::runtime_error("opengl:failed to init 3.0 bindings.");
				}
				if (!gl3wIsSupported(3, 0)) {
					throw std::runtime_error("opengl: Opengl 3.0 not supported.");
				}
				return (window);
			}())
		, rectContext(contextFromFiles("rect"))
	{
		static auto setFrameBuffer = [this] (float w, float h) {
			size = { w, h };
			dim = { h / w, 1.0f };
		};
		glfwSetFramebufferSizeCallback(window.get(), [](GLFWwindow *, int w, int h)
				{
					glViewport(0, 0, w, h);
					setFrameBuffer(static_cast<float>(w), static_cast<float>(h));
				});

		{
			Bind<RenderContext> bind(rectContext);

			glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
		}
	}

	~Display()
	{}

	void render(void) {
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		/* Debug */
		{
			Rect const r{{-0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}};
			displayRect(r);
		}

		glDisable(GL_BLEND);
		glfwSwapBuffers(window.get());
		glfwPollEvents();
	}

	void displayRect(Rect const &rect) {
		Bind<RenderContext> bind(rectContext);
		float buffer[2u * 4u];

		for (unsigned int i{0u}; i != 4u; ++i) {
			Vect<2u, float> const corner{static_cast<float>(i & 1u), static_cast<float>(i >> 1u)};
			Vect<2u, float> const destCorner{corner * rect.size + rect.pos};

			std::copy(&destCorner[0], &destCorner[2u], &buffer[i * 2u]);
		}

		glBindBuffer(GL_ARRAY_BUFFER, rectBuffer);
		my_opengl::setUniform(dim, "dim", rectContext.program);
		my_opengl::setUniform(rect.color, "rect_color", rectContext.program);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	bool isRunning(void) const {
		return (!glfwWindowShouldClose(window.get()));
	}
};

#endif /* !DISPLAY_HPP */
