#include "ThreadPool.h"





void  ThreadPool::AddTask(Task task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(task);
    }
    cond_var.notify_one(); // 通知一个工作线程有新任务
}


unsigned int ThreadPool::WorkerThread(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;
    while (true)
    {
        std::unique_lock<std::mutex>lock(pool->queue_mutex);
        pool->cond_var.wait(lock, [pool] { return pool->stop || !pool->tasks.empty(); });
        if (pool->stop && pool->tasks.empty()) {
            break;
        }
        Task task = pool->tasks.front();
        pool->tasks.pop();
        task(); // 执行任务
    }
    return 0;
}