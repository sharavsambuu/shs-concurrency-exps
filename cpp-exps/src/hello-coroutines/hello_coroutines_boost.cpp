#include <iostream>
#include <boost/coroutine2/all.hpp>

using namespace boost::coroutines2;

void doit(coroutine<void>::push_type& yield) {
    std::cout<<"DOIT : first call"<<std::endl;
    yield();
    std::cout<<"DOIT : second call"<<std::endl;
    yield();
    std::cout<<"DOIT : third call"<<std::endl;
}

void fibonacci(coroutine<int>::push_type& yield) {
    int a=0;
    int b=1;
    while (true) {
        yield(b);
        int temp = a+b;
        a = b;
        b = temp;
    }
}

int main() {
    std::cout<<"#### hello yielding coroutine"<<std::endl;
    
    coroutine<void>::pull_type doit_coroutine(doit);
    std::cout<<"MAIN : first call"<<std::endl;
    doit_coroutine();
    std::cout<<"MAIN : second call"<<std::endl;
    doit_coroutine();
    std::cout<<"MAIN : third call"<<std::endl;

    std::cout<<std::endl;

    std::cout<<"#### hello value returning coroutine using fibonacci sequence"<<std::endl;

    coroutine<int>::pull_type fibonacci_coroutine(fibonacci);
    for (int i=0; i<10; ++i) {
        std::cout<<i<<" -> "<<fibonacci_coroutine.get()<<std::endl;
        fibonacci_coroutine();
    }


    return 0;
}