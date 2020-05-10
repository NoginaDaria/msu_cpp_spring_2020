#include <vector>
#include <cstddef>
#include <functional>
#include <future>
#include <queue>
#include <thread>


class ThreadPool
{
  std::vector<std::thread> threads_;
  std::atomic<bool> flag_;

  std::condition_variable dataReady_;
  std::queue <std::function<void()>> jobs_queve;
  std::mutex m_;

  void process()
  {
    while(true)
    {
      std::unique_lock<std::mutex> lock(m_);
      while (!flag_ && jobs_queve.empty())
        dataReady_.wait(lock);
      if (flag_) break;
      auto task = jobs_queve.front();
      jobs_queve.pop();
      task();
    }
  }
public:
    explicit ThreadPool(size_t poolSize)
    {
      flag_ = false;
      for (size_t i = 0; i < poolSize; ++i)
        threads_.push_back(std::thread(&ThreadPool::process, this));
    }

    ~ThreadPool()
    {
      flag_ = true;
      dataReady_.notify_all();
      for (auto& thread : threads_)
        thread.join();
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
      auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
        (std::bind(func, args...));
      std::lock_guard<std::mutex> lock(m_);
      jobs_queve.emplace([task]() { (*task)(); });
      dataReady_.notify_one();
      return task -> get_future();
    }
};
