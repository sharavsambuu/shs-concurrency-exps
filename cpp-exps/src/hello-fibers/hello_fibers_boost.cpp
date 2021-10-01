#include <iostream>
#include <boost/fiber/all.hpp>

void fn(std::string const& str, int n) {
    for (int i=0; i<n; ++i) {
        std::cout<<str<<" : "<<i<<std::endl;
        boost::this_fiber::yield();
    }
}


int main(int argc, char const *argv[]) {
    auto fiber = boost::fibers::fiber([] () {
        std::cout << "Hello World from Boost::Fiber" << std::endl;
    });
    fiber.join();

    boost::fibers::fiber f1(fn, "hello_fiber", 5);
    f1.join();

    std::cout<<"done."<<std::endl;

    return 0;
}