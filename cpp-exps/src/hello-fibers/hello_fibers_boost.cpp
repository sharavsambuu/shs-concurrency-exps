#include <iostream>
#include <boost/fiber/all.hpp>

int main(int argc, char const *argv[]) {
    auto fiber = boost::fibers::fiber([] () {
        std::cout << "Hello World" << std::endl;
    });
    fiber.join();
    return 0;
}