#include <chrono>
#include <iostream>
#include <coop/task.hpp>
#include <thread>

coop::task_t<> hello_coroutine()
{
    auto thread_id = std::this_thread::get_id();
    std::cout<<"coroutine running on thread_"<<thread_id<<std::endl;

    co_await coop::suspend();

    std::this_thread::sleep_for(std::chrono::milliseconds{50});
}

int main() {
    auto my_thread = std::thread([] {
        std::cout<<"hello from thread"<<std::endl;
        hello_coroutine();
    });
    my_thread.join();
    std::cout<<"Hello from Main thread"<<std::endl;
    return 0;
}