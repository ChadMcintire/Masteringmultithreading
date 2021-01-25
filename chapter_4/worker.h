#pragma once
#ifndef WORKER_H
#define WORKER_H

#include "abstract_request.h"

#include <condition_variable>
#include <mutex>

class Worker {
    private:
    std::condition_variable cv;
    std::mutex mtx;
    std::unique_lock<std::mutex> ulock;
    AbstractRequest* request;
    bool running;
    bool ready;

    public:
    Worker() { running = true; ready = false; ulock = std::unique_lock<std::mutex>(mtx);}
    void run();
    void stop() { running = false; }
    void setRequest(AbstractRequest* request) { this->request = request; 
                                                ready = true;}
    void getCondition(std::condition_variable* &cv);
};
#endif
