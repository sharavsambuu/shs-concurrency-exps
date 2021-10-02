#include <chrono>
#include <iostream>
#include <coop/task.hpp>
#include <thread>


coop::task_t<> game_logic_update() {
    co_await coop::suspend();
    for (long i=0; i<10000000; ++i) {
        co_await coop::suspend(); // lets be nice with each other
    }
}

coop::task_t<> game_physics_update() {
    co_await coop::suspend();
    for (long i=0; i<10000000; ++i) {
        //auto thread_id = std::this_thread::get_id();
        co_await coop::suspend(); // of course i'm nice coroutine
    }
}

coop::task_t<> game_rendering_update() {
    co_await coop::suspend();
    for (long i=0; i<10000000; ++i) {
        co_await coop::suspend(); // letting the others do something useful
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

    std::cout<<"please wait..."<<std::endl;

    auto task = game_loop();
    task.join();

    std::cout<<"now control is on main, so good bye!"<<std::endl;

    return 0;
}