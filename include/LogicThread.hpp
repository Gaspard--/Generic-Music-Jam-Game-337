#ifndef LOGIC_THREAD_HPP
# define LOGIC_THREAD_HPP

# include <thread>
# include "Logic.hpp"

class LogicThread
{
	std::mutex mutex_display;
	std::mutex mutex_pause;
	std::mutex exit_mutex;
	bool exit_bool;

	Logic logic;
	std::thread thread;

public:
	template<class... Args>
	LogicThread(Args &&... args)
		: exit_bool(false)
		, logic(std::forward<Args>(args)...)
		, thread([this]() {
			while ([this]() {
					std::unique_lock<std::mutex> ul(exit_mutex);
					return (!exit_bool);
				}())
			{
				std::unique_lock<std::mutex> a(mutex_display);
				std::unique_lock<std::mutex> b(mutex_pause);
				logic.tick();
			}
		})
	{}

	~LogicThread() {
		exit();
		thread.join();
	}

	std::unique_lock<std::mutex> lock(void) {
		return (std::unique_lock<std::mutex>(mutex_display));
	}

	void pause(void) {
		mutex_pause.lock();
	}

	void unpause(void) {
		mutex_pause.unlock();
	}

	void exit(void) {
		std::unique_lock<std::mutex> l(exit_mutex);
		exit_bool = true;
	}
};

#endif /* !LOGIC_THREAD_HPP */
