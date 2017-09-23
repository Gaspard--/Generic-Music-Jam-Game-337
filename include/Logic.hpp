#ifndef LOGIC_HPP
# define LOGIC_HPP

# include <mutex>

# include <iostream> /* DEBUG */

class Logic
{
	std::mutex mutex;

	std::mutex exit_mutex;
	bool exit_bool;

public:
	Logic();
	~Logic();

	void run(void) {
		while ([this]() {
			std::unique_lock<std::mutex> lg(exit_mutex);
			return (!exit_bool);
		}()) {
		}
	}

	std::unique_lock<std::mutex> lock(void) {
		std::unique_lock<std::mutex> lg(mutex);
		return lg;
	}

	void pause(void) {
		mutex.lock();
	}

	void unpause(void) {
		mutex.unlock();
	}

	void exit(void) {
		std::unique_lock<std::mutex> lg(exit_mutex);
		exit_bool = true;
	}
};

#endif /* !LOGIC_HPP */
