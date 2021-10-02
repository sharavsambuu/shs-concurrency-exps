/*
    Төсөөллийн тоглоомын циклийг coroutine ашиглан хэрэгжүүлж үзэх
    C++20-ийн хувьд coroutine бол stackless буюу компилятороос үүсгэгдэг
    state machine юм
*/

#include <chrono>
#include <iostream>
#include <coop/task.hpp>
#include <thread>


coop::task_t<> game_logic_update() {
    co_await coop::suspend();
    for (int i=0; i<10; ++i) {
        auto thread_id = std::this_thread::get_id();
        std::cout<<"thread_"<<thread_id<<" : game logic coroutine is running..."<<std::endl;
        co_await coop::suspend(); // lets be nice with each other
        std::this_thread::sleep_for(std::chrono::milliseconds{1300});
    }
}

coop::task_t<> game_physics_update() {
    co_await coop::suspend();
    for (int i=0; i<7; ++i) {
        auto thread_id = std::this_thread::get_id();
        std::cout<<"thread_"<<thread_id<<" : game physics coroutine is running..."<<std::endl;
        co_await coop::suspend(); // of course i'm nice coroutine
        std::this_thread::sleep_for(std::chrono::milliseconds{1500});
    }
}

coop::task_t<> game_rendering_update() {
    co_await coop::suspend();
    for (int i=0; i<15; ++i) {
        auto thread_id = std::this_thread::get_id();
        std::cout<<"thread_"<<thread_id<<" : game rendering coroutine is running..."<<std::endl;
        co_await coop::suspend(); // letting the others do something useful
        std::this_thread::sleep_for(std::chrono::milliseconds{700});
    }
}

coop::task_t<void, true> game_loop() {
    auto logic_task     = game_logic_update();
    auto physics_task   = game_physics_update();
    auto rendering_task = game_rendering_update();

    co_await logic_task;
    co_await physics_task;
    co_await rendering_task;
}

int main() {

    std::cout<<"hello from main, let's do some imaginary game loop with coroutines"<<std::endl;

    auto my_thread = std::thread([] {
        std::cout<<"worker thread is starting..."<<std::endl;

        auto task = game_loop();
        task.join();

        std::cout<<"worker thread finished to run coroutine tasks"<<std::endl;
    });
    my_thread.join();

    std::cout<<"now control is on main, so good bye!"<<std::endl;

    return 0;
}