#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <windows.h>
#include <random>
#include <chrono>
// 任务类型，这里简单地使用 std::function<void()>
using Task = std::function<void()>;

class ThreadPool {
public:
    ThreadPool(int threadNumbers) :max_threads(threadNumbers), stop(false)
    {
        for (int i = 0; i < threadNumbers; i++)
        {
            workers.emplace_back(WorkerThread, this);
        }
        startTime = std::chrono::high_resolution_clock::now();
    }
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cond_var.notify_all(); // 通知所有工作线程停止
        for (std::thread& worker : workers) {
            worker.join();
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Total time: " << duration.count() << " ms" << std::endl;
    }

    // 添加新任务到线程池
    void AddTask(Task task);
private:
    //线程的工作函数
    static unsigned int WorkerThread(void* arg);


private:
    std::vector<std::thread> workers;                               // 工作线程池
    std::queue<Task> tasks;                                         // 任务队列
    std::mutex queue_mutex;                                         // 用于保护任务队列的互斥锁
    std::condition_variable cond_var;                               // 条件变量，用于通知工作线程有新任务
    int max_threads;                                                // 线程池的最大线程数
    bool stop;                                                      // 线程池停止标志
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
};

