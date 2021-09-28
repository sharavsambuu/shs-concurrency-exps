#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>


int main(int argc, const char** argv) {
    // энэ компютер дээр яг хэдэн тооцооллын урсгал зэрэгцээ ажиллах боломжтой байна
    unsigned int num_cpus = std::thread::hardware_concurrency();
    std::cout << "hardware threads count : " << num_cpus << std::endl;

    // олон thread-үүдээс std::cout-руу замбараатай хандахад хэрэглэх mutex
    std::mutex iomutex_0;
    std::mutex iomutex_1;
    std::vector<std::thread> threads(num_cpus);

    for (unsigned int i = 0; i < num_cpus; ++i) {
        // lambda функцээр thread үүсгэх, iomutex-ийн урд ampersand тавьдаг нь
        // энэ lambda дотор гаднах scope-ийн iomutex-д хандах боломжтой гэдгийг илтгэнэ
        threads[i] = std::thread([&iomutex_0, &iomutex_1, i] {
            // энэ scope дотор mutex дээр үүсгэсэн lock хүчинтэй
            {
                std::lock_guard<std::mutex> iolock(iomutex_0);
                std::cout << "thread #" << i << " is running..." << std::endl;
            }

            // чухал юм thread-ээр хийлгэж байгаа юм шиг болгохоор sleep тавих
            int sleep_duration = rand() % (7000 - 1000 + 1) + 2000;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));

            // дахин юм хэвлэх гээд scope авлаа
            {
                std::lock_guard<std::mutex> iolock(iomutex_1);
                std::cout << "thread #" << i << " is done to do its job." << std::endl;
            }
        });
    }

    std::cout << "hello from main thread..." << std::endl;

    // үүсгэсэн thread-үүдээ ажиллаж дуустал нь хүлээх
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "hello again, main thread is done to do its job as well, bye!" << std::endl;

    return 0;
}
