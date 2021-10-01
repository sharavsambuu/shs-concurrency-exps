/*
    Олон fiber-үүдийг олон thread-ээр хувааж ажиллуулах.
    процессоруудын цөм хооронд fiber-үүд зөөх нь жаахан үрэлгэн байж магадгүй

    Boost-ийн fiber гэдэг бол тус бүрдээ өөр өөрсдийн гэсэн stack-тэй, CPU register, 
    flag, instruction pointer, stack pointer гэх мэт ажиллагааг хадгалах шаардлагатай
    бүх төлвүүдийг агуулдаг тул дурын процессор цөм дээр зөөвөрлөгдөн ажиллах
    боломжтой байдаг. Өөрөөр хэлбэл ямар нэгэн thread-д харъяалалгүйгээр бусад
    thread-үүд дээр зөөн ажиллуулж болдог гэсэн үг.

    https://www.boost.org/doc/libs/1_77_0/libs/fiber/doc/html/fiber/migration.html
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <chrono>
#include <condition_variable>
#include <boost/fiber/all.hpp>
#include <boost/fiber/barrier.hpp>

using namespace std::chrono_literals;

static std::size_t fiber_count{0};
static std::mutex mutex_count{};
typedef std::unique_lock<std::mutex> lock_type;
static boost::fibers::condition_variable_any condition_count{};



void worker_fiber(char me) {
    std::thread::id thread_id = std::this_thread::get_id();
    {
        std::ostringstream buffer;
        buffer<<"fiber "<<me<<" is started on thread "<<thread_id<<"\n";
        std::cout<<buffer.str()<<std::flush;
    }

    for (int i=0; i<100; ++i) {
        //boost::this_fiber::sleep_for(200ms);
        boost::this_fiber::yield();

        // энэ fiber-ийг өөр thread дээр шилжүүлсэн эсэхийг илрүүлэх
        std::thread::id new_thread_id = std::this_thread::get_id();
        if (new_thread_id!=thread_id) {
            thread_id = new_thread_id;
            std::ostringstream buffer;
            buffer<<"fiber "<<me<<" is switched to thread "<<thread_id<<"\n";
            std::cout<<buffer.str()<<std::flush;
        }
    }

    lock_type lock(mutex_count);
    // fiber ажиллах дуус бүрт fiber_count-н утгыг бууруулах
    if (0==--fiber_count) {
        lock.unlock();
        condition_count.notify_all();
    }
}


void worker_thread(boost::fibers::barrier * b) {
    std::ostringstream buffer;
    buffer<<"thread "<<std::this_thread::get_id()<<" is started"<<std::endl;
    std::cout<<buffer.str()<<std::flush;

    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();
    // бусад thread-үүдийг ажиллуулах боломж олгох
    b->wait();

    lock_type lock(mutex_count);
    condition_count.wait(lock, []() {
        return 0==fiber_count;
    });
}


int main() {
    std::cout<<"main thread "<<std::this_thread::get_id()<<" is started"<<std::endl;

    boost::fibers::use_scheduling_algorithm<boost::fibers::algo::shared_work>();

    for (char c : std::string("abcdefghijklmnopqrstuvwxyz")) {
        boost::fibers::fiber([c]() {
            worker_fiber(c);
        }).detach();
        ++fiber_count;
    }

    boost::fibers::barrier b(4);
    // хоорондоо ажил хувааж хийх боломжтой thread-үүд асаах
    std::thread threads[] = {
        std::thread(worker_thread, &b),
        std::thread(worker_thread, &b),
        std::thread(worker_thread, &b)
    };
    b.wait();
    {
        lock_type lock(mutex_count);
        /*
        main fiber-г suspend хийлгээд worker fiber-г ажиллагаанд оруулна
        хэрэв дараах нөхцөл биелбэл main fiber дээр resume хийгдэнэ
        өөрөөр хэлбэл бүх fiber-ууд ажиллаж дууссан гэсэн үг
        */
        condition_count.wait(lock, []() {
            return 0==fiber_count;
        });
    }
    // дээрхи lock release хийгдсэний дараа л thread-үүдээ join хийхгүй бол
    // бусад thread-үүд condition_count дотор түгжигдэж deadlock хийгдэх боломжтой
    for (std::thread & t : threads) {
        t.join();
    }

    std::cout<<"done, bye."<<std::endl;

    return 0;
}