/*
    future механиз хэрэглэж thread-үүд үүсгэж синхрон хийж үзэх
*/
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <vector>

int main() {

    auto future = std::async(std::launch::async, []() {
        auto thread_id = std::this_thread::get_id();
        std::cout<<"thread_"<<thread_id<<" is started"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout<<"thread_"<<thread_id<<" is finished"<<std::endl;

    });
    future.wait();


    std::vector<std::future<void>> futures;
    for (size_t i=0; i<10; ++i) {
        futures.emplace_back(
            std::async(
                std::launch::async, 
                [](size_t param) {
                    auto thread_id = std::this_thread::get_id();
                    std::cout<<"thread_"<<thread_id<<" : "<<param<<" is started"<<std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                    std::cout<<"thread_"<<thread_id<<" : "<<param<<" is finished"<<std::endl;
                },
                i
            )
        );
    }

    for (auto & future : futures) {
        future.wait();
    }


    return 0;
}