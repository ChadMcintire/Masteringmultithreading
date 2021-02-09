#include <iostream>
#include <functional>
#include <thread>
#include <future>

void print_int (std::future<int>& fut) {
    int x = fut.get();
    std::cout << "value: " << x << '\n';
}

//can also use wait_for() and wait_until()
int main () {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread th1 (print_int, std::ref(fut));
    //the promise is completed when the value is set
    prom.set_value (10);
    th1.join();
    return 0;
}

