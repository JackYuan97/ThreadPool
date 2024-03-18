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
// �������ͣ�����򵥵�ʹ�� std::function<void()>
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
        cond_var.notify_all(); // ֪ͨ���й����߳�ֹͣ
        for (std::thread& worker : workers) {
            worker.join();
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "Total time: " << duration.count() << " ms" << std::endl;
    }

    // ����������̳߳�
    void AddTask(Task task);
private:
    //�̵߳Ĺ�������
    static unsigned int WorkerThread(void* arg);


private:
    std::vector<std::thread> workers;                               // �����̳߳�
    std::queue<Task> tasks;                                         // �������
    std::mutex queue_mutex;                                         // ���ڱ���������еĻ�����
    std::condition_variable cond_var;                               // ��������������֪ͨ�����߳���������
    int max_threads;                                                // �̳߳ص�����߳���
    bool stop;                                                      // �̳߳�ֹͣ��־
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
};

