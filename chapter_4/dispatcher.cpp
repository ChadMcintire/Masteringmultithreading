#include "dispatcher.h"

#include <iostream>

std::queue<AbstractRequest*> Dispatcher::requests;
std::queue<Worker*> Dispatcher::workers;
std::mutex Dispatcher::requestsMutex;
std::mutex Dispatcher::workersMutex;
std::vector<Worker*> Dispatcher::allWorkers;
std::vector<std::thread*> Dispatcher::threads;

bool Dispatcher::init(int workers) {
    std::thread* t = 0;
    Worker* w = 0;
    for (int i = 0; i < workers; ++i){
        w = new Worker;
        allWorkers.push_back(w);
        t = new std::thread(&Worker::run, w);
        threads.push_back(t);
        }
    return true;
}

bool Dispatcher::stop() {
    for (int i = 0; i < allWorkers.size(); ++i) {
        allWorkers[i]->stop();
    }
    std::cout << "Stopped workers.\n";
    for (int j = 0; j < threads.size(); ++j) {
        threads[j]->join();
            std::cout << "Joined threads.\n";
    }
}

void Dispatcher::addRequest(AbstractRequest* request) {
    workersMutex.lock();
    if (!workers.empty()) {
        Worker* worker = workers.front();
        worker->setRequest(request);
        std::condition_variable* cv;
        worker->getCondition(cv);
        cv->notify_one();
        workers.pop();
        workersMutex.unlock();
    }
    else {
        workersMutex.unlock();
        requestsMutex.lock();
        requests.push(request);
        requestsMutex.unlock();
    }
}

bool Dispatcher::addWorker(Worker* worker) {
    bool wait = true;
    requestsMutex.lock();
    if (!requests.empty()) {
        AbstractRequest* request = requests.front();
        worker->setRequest(request);
        requests.pop();
        wait = false;
        requestsMutex.unlock();
    }
    else {
        requestsMutex.unlock();
        workersMutex.lock();
        workers.push(worker);
        workersMutex.unlock();
    }
        return wait;
}
