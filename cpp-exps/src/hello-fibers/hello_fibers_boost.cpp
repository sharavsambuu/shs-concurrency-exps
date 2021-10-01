#include <iostream>
#include <boost/fiber/all.hpp>

using namespace boost::fibers;
using namespace boost::this_fiber;


int main() {
    std::cout<<"#### hello fibers"<<std::endl;

    return 0;
}