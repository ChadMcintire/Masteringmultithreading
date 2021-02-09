#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

std::mutex values_mtx;
std::mutex cout_mtx;
std::vector<int> values;

int randGen(const int& min, const int& max){
    // static thread_local makes it so that every thread uses it's own generator
    // hash by the thread value
    // this is the standard procedure for creating a random number generator
    // example here https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c
    static thread_local std::mt19937 generator(std::hash<std::thread::id>()(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void threadFnc(int tid) {
   cout_mtx.lock();
   std::cout << "Starting thread " << tid << ".\n";
   cout_mtx.unlock();

   values_mtx.lock();
   int val = values[0];
   values_mtx.unlock();

   int rval = randGen(0, 10);
   val += rval;

   cout_mtx.lock();
   std::cout << "Thread " << tid << " adding " << rval << ". New value: " << val    << ".\n";

   cout_mtx.unlock();

   values_mtx.lock();
   values.push_back(val);
   values_mtx.unlock();
}

int main() {
    values.push_back(42);

    std::thread tr1(threadFnc, 1);
    std::thread tr2(threadFnc, 2);
    std::thread tr3(threadFnc, 3);
    std::thread tr4(threadFnc, 4);

    tr1.join();
    tr2.join();
    tr3.join();
    tr4.join();

    std::cout << "Input: " << values[0] << ", Result 1: " << values[1] 
    << ", Result 2: " << values[2] << ", Result 3: " << values[3] 
    << ", Result 4: " << values[4] << "\n";

}    
