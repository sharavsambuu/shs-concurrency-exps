/*
    Future объект дээр native co_await яаж хэрэглэх талаар гайхав

    - https://stackoverflow.com/questions/61230277/future-with-coroutines-co-await
*/

#include <cassert>
#include <coroutine>
#include <future>
#include <iostream>
#include <optional>
#include <thread>

using namespace std::literals;


template <class T>
class FutureAwaitable {
public:
    template <class U> 
    struct BasicPromiseType {
        auto get_return_object() {
            return FutureAwaitable<T>(CoroHandle::from_promise(*this));
        }
        std::suspend_always initial_suspend() noexcept {
            return {};
        }
        std::suspend_never final_suspend() noexcept {
            return {};
        }
        template <class V>
        requires std::is_convertible_v<V, T>
        void return_value(V v) { _value = v; }
        void unhandled_exception() { throw; }
        std::optional<T> _value;
    };
    using promise_type = BasicPromiseType<FutureAwaitable<T>>;
    using CoroHandle = std::coroutine_handle<promise_type>;
    explicit FutureAwaitable(CoroHandle h) : _parent(h) {  }
    ~FutureAwaitable() {}
    bool is_ready() const {
        auto & fut = std::get<FutureAwaitable<T> *>(&_parent);
        return fut->wait_for(std::chrono::seconds(0)) != std::future_status::ready;
    }
    FutureAwaitable(std::future<T> && f) {
        _f = &f;
    }
    T get() const { return promise()._value.value(); }
    std::future<T> & std_future() const {
        assert(_f->valid());
        return *_f;
    }
    bool await_ready() {
        if (!(_f->wait_for(std::chrono::seconds(0)) == std::future_status::ready)) {
            return true;
        }
        return false;
    }
    auto await_resume() {
        return std_future().get();
    }
    bool await_suspend(CoroHandle parent) {
        _parent = parent;
        return true;
    }
    void resume() {
        assert(_parent);
        _parent.resume();
    }
    auto parent() const { return _parent; }
    bool done() const noexcept {
        return _parent.done();
    }
private:
    auto & promise() const noexcept { return _parent.promise(); }
    CoroHandle _parent = nullptr;
    std::future<T> * _f = nullptr;
};
template <class T> auto operator co_await(std::future<T> &&f) {
  return FutureAwaitable<T>(std::forward<std::future<T>>(f));
}
template <class T> auto operator co_await(std::future<T> & f) {
  return FutureAwaitable<T>(std::forward<std::future<T>>(f));
}


FutureAwaitable<int> futureable_coroutine() {
    auto computation1 = std::async(std::launch::async, [] {
        int sum = 0;
        for (int i=1; i<=5; ++i) {
            sum += i;
            std::cout<<"computation1 is doing summation calculation"<<std::endl;
            std::this_thread::sleep_for(1s);
        }
        return sum;
    });
    auto computation2 = std::async(std::launch::async, [] {
        int result = 1;
        for (int i=1; i<=6; ++i) {
            result = result*i;
            std::cout<<"computation2 is doing multiplication calculation"<<std::endl;
            std::this_thread::sleep_for(1s);
        }
        return result;
    });

    int result1 = co_await computation1;
    std::cout<<"computation result1 : "<<result1<<std::endl;

    int result2 = co_await computation2;
    std::cout<<"computation result2 : "<<result2<<std::endl;

    co_return co_await std::async(
        std::launch::async, 
        [] {
            int j = 0;
            for (int i = 0; i < 1000; ++i) { j += i; }
            return j;
        });
}


int main() {

    std::cout<<"let's await some futures"<<std::endl;

    FutureAwaitable<int> fcoro = futureable_coroutine();
    do { fcoro.resume(); } while(!fcoro.done());

    std::cout<<"coroutine returned value : "<<fcoro.get()<<std::endl;

    return 0;
}