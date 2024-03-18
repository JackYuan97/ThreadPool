#include "ThreadPool.h"
void  ThreadPool::AddTask(Task task,int hashIndex) {
    {
        std::unique_lock<std::mutex> lock(LocalMutex[hashIndex]);
        LocalTask[hashIndex].push_back(task);
        //tasks.push(task);
    }
    cond_var.notify_one(); // 通知一个工作线程有新任务
}


void ThreadPool::WorkerThread(void* arg, int threadIndex)
{
    ThreadPool* pool = (ThreadPool*)arg;
    while (true)
    {
        std::unique_lock<std::mutex>lock(pool->LocalMutex[threadIndex]);
        pool->cond_var.wait(lock, [pool, threadIndex] { return pool->stop[threadIndex] || !pool->LocalTask[threadIndex].empty(); });
        if (pool->stop[threadIndex] && pool->LocalTask[threadIndex].empty()) {
            break;
        }
        Task task = pool->LocalTask[threadIndex].front();
        pool->LocalTask[threadIndex].pop_front();
        task(); // 执行任务
    }

}