#include "tp.h"
#include <stdexcept>

ThreadPool::ThreadPool(std::size_t thread_count)
    : stopping_(false){
    if(thread_count==0){
        thread_count=1;
    }
    for(std::size_t i = 0; i<thread_count; i++){
        workers_.push_back(std::thread(&ThreadPool::WorkerLoop, this));            
    }
}

ThreadPool::~ThreadPool(){
    Shutdown();
}


void ThreadPool::WorkerLoop(){
    std::function<void()> task;

    for(;;){
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this](){
                return stopping_ || !tasks_.empty();
            });

            if(stopping_ && tasks_.empty()){
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop_front();
        }

        task();
    }
}

void ThreadPool::JoinWorkers(){
    for(std::thread& worker : workers_){
        if(worker.joinable()){
            worker.join();
        }
    }
}

void ThreadPool::Shutdown(){
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }
    cv_.notify_all();
    JoinWorkers();
}