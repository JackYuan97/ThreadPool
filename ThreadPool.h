#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <random>
#include <chrono>
#include <windows.h>
 //任务类型，这里简单地使用 std::function<void()>
using Task = std::function<void()>;

class ThreadPool {
public:
    ThreadPool(int threadNumbers)
        :max_threads(threadNumbers), 
        startTime(std::chrono::high_resolution_clock::now()),
        LocalMutex(threadNumbers), 
        LocalTask(threadNumbers),
        stop(threadNumbers)
    {
        for (int i = 0; i < threadNumbers; i++)
        {
            workers.emplace_back(WorkerThread, this, i);
        }
    }
    ~ThreadPool()
    {
        {
            for (size_t i = 0; i < max_threads; i++)
            {
                std::unique_lock<std::mutex> lock(LocalMutex[i]);
                stop[i] = true;
            }

        }
        cond_var.notify_all(); // 通知所有工作线程停止
        for (std::thread& worker : workers) {
            worker.join();
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
        std::cout << "Total time: " << duration.count() << " ms" << std::endl;
    }

    // 添加新任务到线程池
    void AddTask(Task task, int hashIndex);
private:
    //线程的工作函数
    static void WorkerThread(void* arg,int threadIndex);

private:
    std::vector<std::thread> workers;                               // 工作线程池
    std::queue<Task> tasks;                                         // 任务队列
    std::mutex queue_mutex;                                         // 用于保护任务队列的互斥锁
    std::condition_variable cond_var;                               // 条件变量，用于通知工作线程有新任务
    int max_threads;                                                // 线程池的最大线程数
    std::vector<bool> stop;                                         // 线程池停止标志
    std::vector<std::deque<Task>>  LocalTask;                       // 线程内部维护的任务队列
    std::vector<std::mutex> LocalMutex;                             // 子线程内部的互斥锁
    std::chrono::high_resolution_clock::time_point startTime;
};

