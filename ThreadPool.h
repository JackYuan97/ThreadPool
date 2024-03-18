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
 //�������ͣ�����򵥵�ʹ�� std::function<void()>
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
        cond_var.notify_all(); // ֪ͨ���й����߳�ֹͣ
        for (std::thread& worker : workers) {
            worker.join();
        }
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime);
        std::cout << "Total time: " << duration.count() << " ms" << std::endl;
    }

    // ����������̳߳�
    void AddTask(Task task, int hashIndex);
private:
    //�̵߳Ĺ�������
    static void WorkerThread(void* arg,int threadIndex);

private:
    std::vector<std::thread> workers;                               // �����̳߳�
    std::queue<Task> tasks;                                         // �������
    std::mutex queue_mutex;                                         // ���ڱ���������еĻ�����
    std::condition_variable cond_var;                               // ��������������֪ͨ�����߳���������
    int max_threads;                                                // �̳߳ص�����߳���
    std::vector<bool> stop;                                         // �̳߳�ֹͣ��־
    std::vector<std::deque<Task>>  LocalTask;                       // �߳��ڲ�ά�����������
    std::vector<std::mutex> LocalMutex;                             // ���߳��ڲ��Ļ�����
    std::chrono::high_resolution_clock::time_point startTime;
};

