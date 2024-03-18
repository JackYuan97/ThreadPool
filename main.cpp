#include"ThreadPool.h"


int random()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // ����һ���ֲ������ڶ���������ķ�Χ
    std::uniform_int_distribution<> distr(1000, 2000);

    // ���������
    int random_number = distr(gen);
    return random_number;
}


// ʾ��������
void DoWork(int id) {
    std::cout << "Task " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
    //Sleep(random()); // ģ���ʱ����
    Sleep(10);
}

int main() {

    const int num_tasks = 500;
    const int num_threads = 4;

    ThreadPool pool(num_threads);

    // ��������̳߳�
    for (int i = 0; i < num_tasks; ++i) {
        pool.AddTask(std::bind(DoWork, i));
    }

    // �ȴ������������
    //pool.AddTask([]() {
    //    std::cout << "All tasks are completed." << std::endl;
    //    });



    return 0;
}