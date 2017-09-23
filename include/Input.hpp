#ifndef INPUT_HPP_
# define INPUT_HPP_

# include "my_glfw.hpp"

# include <iostream>
# include <queue>

struct Key {
  int key;
  int scancode;
  int action;
  int mode;
};

struct Mouse {
  double x;
  double y;
};

struct Button {
  int button;
  int action;
  int mods;
};

struct Event
{
  bool hasEvent;
  enum {
    KEY,
    MOUSE,
    BUTTON,
  } type;

  GLFWwindow *window;

  union {
    Key key;
    Mouse mouse;
    Button button;
  } val;

  operator bool() const { return hasEvent; };
};

/**
 * Set callback with logic
 */
class Input
{
public:
  Input()
  {
    inputStatic(this, true);
  }

  ~Input()
  {
    inputStatic(nullptr, true);
  }

  Event pollEvent()
  {
    if (events.empty())
      {
	Event ev;
	ev.hasEvent = false;
	return ev;
      }

    Event event = events.front();

    events.pop();

    return event;
  }

  static Input *inputStatic(Input *input = nullptr, bool set = false)
  {
    static Input *stored{nullptr};

    if (!set)
      return stored;
    return stored = input;
  }

  static void setCallbacksForWindow(GLFWwindow *window)
  {
    // set glfw callback
    glfwSetKeyCallback(window, [] (GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (Input *input = inputStatic())
	  {
	    Event ev = {true, Event::KEY, window, {.key = {key, scancode, action, mode}}};

	    input->events.push(ev);
	  }
      });
    glfwSetCursorPosCallback(window, [] (GLFWwindow *window, double x, double y) {
	if (Input *input = inputStatic())
	  {
	    Event ev = {true, Event::MOUSE, window, {.mouse = {x, y}}};
	    input->events.push(ev);
	  }
      });
    glfwSetMouseButtonCallback(window, [] (GLFWwindow *window, int button, int action, int mods) {
	if (Input *input = inputStatic())
	  {
	    Event ev = {true, Event::BUTTON, window, {.button = {button, action, mods}}};

	    input->events.push(ev);
	  }
      });
  }

      // bool isKeyPressed(int key);

  private:
      std::queue<Event> events;
  };

#endif /* !INPUT_HPP_ */
