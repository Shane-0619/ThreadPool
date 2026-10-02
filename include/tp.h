#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <utility>
#include <future>
#include <memory>

class ThreadPool{
    public:
    ThreadPool(std::size_t thread_count);
    ~ThreadPool();

    template<class F>
    std::future<std::invoke_result_t<F>> Submit(F task){
        using R = std::invoke_result_t<F>;
        auto ptask = std::make_shared<std::packaged_task<R()>>(task);

        std::future<R> future = ptask->get_future();
        std::function<void()> wrapper([ptask](){
            (*ptask)();
        });

        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(stopping_){
            throw std::runtime_error("ThreadPool has been stopped"); 
            }
            tasks_.push_back(std::move(wrapper));
        }

        cv_.notify_one();
        return future;
    }

    void Shutdown();

    private:
    void WorkerLoop();
    void JoinWorkers();

    private:
    std::vector<std::thread> workers_;
    std::deque<std::function<void()>> tasks_;

    std::condition_variable cv_;
    std::mutex mutex_;

    bool stopping_;
};