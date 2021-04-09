#include "thread_pool.h"
#include <spdlog/spdlog.h>
#include <chrono>

int main() {

    crp::ThreadPool thread_pool;

    for(int i = 0; i < 40; ++i) {
        auto task = std::function<void(int)>([](int i){
            spdlog::info("task {} starts", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            spdlog::info("task {} completed", i);
        });
        thread_pool.add_task(std::bind(task, i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    thread_pool.stop();

    spdlog::info("bye");

    return 0;
}
