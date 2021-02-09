#include <iostream>
#include <future>
#include <chrono>
#include <thread>

int countdown(int from, int to) {
    for(int i = from; i != to; --i) {
        std::cout << i << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Finished countdown.\n";
    return from - to;
}

int main() {
    //allows asynchronous execution, and returns a future object
    std::packaged_task<int(int, int)> task(countdown);
    std::future<int> result = task.get_future();
    std::thread t (std::move(task), 10, 0);

    int value = result.get();

    std::cout << "The countdown lasted for " << value << " seconds.\n";

    t.join();
    return 0;
}
