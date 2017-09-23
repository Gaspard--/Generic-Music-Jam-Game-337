#include "LogicThread.hpp"

LogicThread::~LogicThread()
{
	logic.exit();
	thread.join();
}
