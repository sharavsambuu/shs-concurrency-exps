#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <thread>
#include <chrono>

struct HelloWorldCoro {
    struct promise_type { // compiler looks for `promise_type`
        HelloWorldCoro get_return_object() { return this; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() {}
    };
    HelloWorldCoro(promise_type* p) : m_handle(std::coroutine_handle<promise_type>::from_promise(*p)) {}
    ~HelloWorldCoro() { m_handle.destroy(); }
    void resume() { m_handle.resume(); }
    bool done() const noexcept { return m_handle.done(); }
    std::coroutine_handle<promise_type> m_handle;
};

HelloWorldCoro hello_coroutine(const std::string name, int n) {
    for (int i=0; i<n; ++i) {
        std::cout<<name<<"   : call "<<i<<std::endl;
        co_await std::suspend_always{};
    }
}

HelloWorldCoro another_coroutine() {
    for (int i=0; i<20; ++i) {
        std::cout<<"another : call "<<i<<std::endl;
        co_await std::suspend_always{};
    }
}

int main() {
    std::cout<<"hello from main"<<std::endl;

    HelloWorldCoro coro1   = hello_coroutine("coro1", 4);
    HelloWorldCoro coro2   = hello_coroutine("coro2", 9);
    HelloWorldCoro another = another_coroutine();
    do { 
        if (!coro1.done())   coro1.resume();
        if (!coro2.done())   coro2.resume();
        if (!another.done()) another.resume();
    } while(!coro1.done() || !coro2.done() || !another.done());
    

    return 0;
}
