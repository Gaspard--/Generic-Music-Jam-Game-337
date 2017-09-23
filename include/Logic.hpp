#ifndef LOGIC_HPP
# define LOGIC_HPP

# include <mutex>

# include <iostream> /* DEBUG */

struct Logic
{
	constexpr Logic() {}
	~Logic() {}

	void tick(void) {
		std::cout << "tick!" << std::endl;
	}
};

#endif /* !LOGIC_HPP */
