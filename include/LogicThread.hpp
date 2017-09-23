#ifndef LOGIC_THREAD_HPP
# define LOGIC_THREAD_HPP

# include <thread>
# include "Logic.hpp"

class LogicThread
{
	Logic logic;
	std::thread thread;

public:
	template<class... Args>
	LogicThread(Args &&... args)
		: logic(std::forward<Args>(args)...)
		, thread([this]() {
			logic.run();
		})
	{}

	~LogicThread();
};

#endif /* !LOGIC_THREAD_HPP */
