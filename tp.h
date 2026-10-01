#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <utility>

class ThreadPool{
    public:
    ThreadPool(std::size_t thread_count);
    ~ThreadPool();
    void Submit(std::function<void()> task);
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