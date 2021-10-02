/*
    Олон thread ашиглахад дундын нөөц хэрэглэхэд гардаг асуудал
    Олон удаа ажиллуулж үзэх хэрэгтэй
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>

struct UnsafeCounter {
    int value;
    UnsafeCounter() : value(0) {}
    void increment() { ++value; }
};

struct ThreadsafeCounter {
    std::mutex mutex;
    int value;
    ThreadsafeCounter() : value(0) {}
    void increment() {
        // олон thread-үүдээр зэрэг нөөцөд зэрэг хандахаас сэргийлж lock тавих
        mutex.lock();
        ++value;
        mutex.unlock();
    }
};

int main() {
    UnsafeCounter     unsafe_counter;
    ThreadsafeCounter threadsafe_counter;

    std::vector<std::thread> threads;
    for (int i=0; i<500; ++i) {
        threads.push_back(std::thread([&unsafe_counter, &threadsafe_counter]() {
            for (int i=0; i<100; ++i) {
                unsafe_counter.increment();
                threadsafe_counter.increment();
            }
        }));
    }
    for (auto& thread : threads) { thread.join(); }

    std::cout<<"run this program many times in order to see the threading effect"<<std::endl<<std::endl;;
    std::cout<<"should be exactly 500*100=50000, but let's see the result"<<std::endl;

    std::cout<<"unsafe counter value : "<<unsafe_counter.value<<std::endl<<std::endl;

    std::cout<<"but what about the thread safe counter result?"<<std::endl;
    std::cout<<"thread safe counter value : "<<threadsafe_counter.value<<std::endl;

    return 0;
}