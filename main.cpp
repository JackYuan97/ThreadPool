#include"ThreadPool.h"



int random()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // 创建一个分布，用于定义随机数的范围
    std::uniform_int_distribution<> distr(1000, 2000);

    // 生成随机数
    int random_number = distr(gen);
    return random_number;
}


// 示例任务函数
void DoWork(int id) {
    std::cout << "Task " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
    //Sleep(random()); // 模拟耗时任务
    Sleep(10);
}

int main() {

    const int num_tasks = 500;
    const int num_threads = 4;

    ThreadPool pool(num_threads);

    // 添加任务到线程池
    for (int i = 0; i < num_tasks; ++i) {
        int hashIndex = i % num_threads;
        pool.AddTask(std::bind(DoWork, i), hashIndex);
    }


    return 0;
}