#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>

struct ReturnObject {
    struct promise_type {
        promise_type() = default;
        ReturnObject get_return_object() { return {}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_never final_suspend() const noexcept { return{}; }
        void return_void() noexcept {}
        void unhandled_exception() {}
    };
};

struct HelloWorldCoro {
    struct promise_type { // compiler looks for `promise_type`
        HelloWorldCoro get_return_object() { return this; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept{ return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
    HelloWorldCoro(promise_type* p) : m_handle(std::coroutine_handle<promise_type>::from_promise(*p)) {}
    ~HelloWorldCoro() { m_handle.destroy(); }
    std::coroutine_handle<promise_type> m_handle;
};

HelloWorldCoro hello_coroutine() {
    std::cout << "Hello" << std::endl;
    co_await std::suspend_always{};
    std::cout << "World" << std::endl;
}

int main() {
    HelloWorldCoro my_coroutine = hello_coroutine();
    std::cout << "first coroutine call :" << std::endl;
    my_coroutine.m_handle.resume();
    std::cout << "second coroutine call :" << std::endl;
    my_coroutine.m_handle.resume();
    
    return 0;
}
