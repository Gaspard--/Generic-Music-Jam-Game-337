#include <iostream>
#include "Display.hpp"

int main(void) {
	try {
		Display<1920, 1080> display;
		while (display.isRunning()) {
			display.render();
		}
	} catch (std::runtime_error const &e) {
		std::cerr << "Program encountered a fatal error: " << std::endl << e.what() << std::endl;
		return (-1);
	}
	return (0);
}
