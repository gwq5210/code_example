/**
 * @file a.cpp
 * @brief 测试验证promise future用于通知
 * 线上相关代码： https://git.woa.com/search_plat/xsearch_config/reviews/50
 * g++ a.cc -lpthread
 */
#include <unistd.h>

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main() {
  auto start_commit = std::chrono::steady_clock::now().time_since_epoch();

  std::cout << "main thread:" << std::this_thread::get_id() << std::endl;

  std::promise<void> promise;
  std::future<void> future = promise.get_future();

  auto async_future = std::async(std::launch::async, [&future] {
    std::cout << "work thread:" << std::this_thread::get_id() << std::endl;
    while (true) {
      // 这里如果用sleep则会导致阻塞，需要事件通知使得及时停止等待，C++11之前一般采用条件变量+锁实现，现代C++可以采用p-f
      if (future.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {
        break;
      }
      std::cout << "do work" << std::endl;
    }
  });

  std::cout << "main thread waiting async jobs" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  promise.set_value();  // 通知工作线程退出
  async_future.get();  // 等待工作线程退出

  auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
  std::cout << "cost ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count()
            << std::endl;

  return 0;
}
