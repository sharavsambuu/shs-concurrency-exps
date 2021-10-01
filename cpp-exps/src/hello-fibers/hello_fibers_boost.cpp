#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <chrono>
#include <condition_variable>
#include <boost/fiber/all.hpp>
#include <boost/fiber/barrier.hpp>


void fn(std::string const& str, int n) {
    for (int i=0; i<n; ++i) {
        std::cout<<str<<" : "<<i<<std::endl;
        boost::this_fiber::yield();
    }
}


int value_1 = 0;
int value_2 = 0;

void cooperative_fn1() {
    boost::fibers::fiber::id id = boost::this_fiber::get_id();
    for (int i=0; i<10; ++i) {
        value_1++;
        std::cout<<"fiber_1 "<<id<<" value_1 : "<<value_1<<std::endl;
        boost::this_fiber::yield();
    }
    std::cout<<"fiber_1 is done to do its job."<<std::endl;
}

void cooperative_fn2() {
    boost::fibers::fiber::id id = boost::this_fiber::get_id();

    for (int i=0; i<20; ++i) {
        std::cout<<"fiber_2 "<<id<<" value_2 : "<<value_2<<std::endl;
        value_2++;
        boost::this_fiber::yield(); // be nice to each other
    } 
    std::cout<<"fiber_2 is done to do its job."<<std::endl;
}

int main() {
    auto fiber = boost::fibers::fiber([] () {
        std::cout << "Hello World from Boost::Fiber" << std::endl;
    });
    fiber.join();

    boost::fibers::fiber f1(fn, "hello_fiber", 5);
    f1.join();


    boost::fibers::fiber cooperative_fiber1(cooperative_fn1);
    boost::fibers::fiber cooperative_fiber2(cooperative_fn2);

    cooperative_fiber1.join();
    cooperative_fiber2.join();

    std::cout<<"done."<<std::endl;

    return 0;
}