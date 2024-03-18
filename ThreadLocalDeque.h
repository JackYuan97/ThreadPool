#pragma once
#include<deque>
#include"ThreadPool.h"

class ThreadLocalDeque
{

private:


private:
	std::deque<Task>  LocalTask; //线程内部维护的任务队列
};