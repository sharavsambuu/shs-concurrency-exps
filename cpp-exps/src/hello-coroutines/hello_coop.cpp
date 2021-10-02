#include <chrono>
#include <iostream>
#include <coop/task.hpp>
#include <thread>

coop::task_t<void, true> joinable_coroutine()
{
    auto thread_id = std::this_thread::get_id();
    std::cout<<"coroutine running on thread_"<<thread_id<<std::endl;

    co_await coop::suspend();

    std::this_thread::sleep_for(std::chrono::milliseconds{2000});

    std::cout<<"joinable coroutine is finished"<<std::endl;
}

int main() {
    auto my_thread = std::thread([] {
        std::cout<<"hello from thread"<<std::endl;
        auto task = joinable_coroutine();
        task.join();
        std::cout<<"this thread is finished to run coroutines"<<std::endl;
    });
    my_thread.join();
    std::cout<<"Hello from Main thread"<<std::endl;
    return 0;
}