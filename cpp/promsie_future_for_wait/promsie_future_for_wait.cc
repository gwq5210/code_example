/**
 * @file promise_future_for_wait.cpp
 * @brief 测试验证promise future用于等待标记，适用场景：工作线程需要等待主线程干完某些事情之后，才能干活。
 * 此时的promise-future相当于一个带有wait功能的flag
 */
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

// 干活函数
void DoJob(const std ::future<void>& future) {
  std::cout << "work thread:" << std::this_thread::get_id() << std::endl;
  // 干活之前需要先确保前置工作已经干完了
  future.wait();
  std::cout << "wait finish" << std::endl;
}

int main() {
  auto start_commit = std::chrono::steady_clock::now().time_since_epoch();

  std::cout << "main thread:" << std::this_thread::get_id() << std::endl;

  std::promise<void> promise;
  std::future<void> future = promise.get_future();

  // 模拟多个工作线程并行干活
  auto async_future = std::async(std::launch::async, [&future] { DoJob(future); });
  auto async_future2 = std::async(std::launch::async, [&future] { DoJob(future); });
  auto async_future3 = std::async(std::launch::async, [&future] { DoJob(future); });

  std::cout << "main thread waiting async jobs" << std::endl;
  // 模拟主线程干活
  std::this_thread::sleep_for(std::chrono::seconds(3));
  // 干活完成设置完成标记
  promise.set_value();

  // 下面这三是等待工作线程干完活，主线程再往下走
  async_future.get();
  async_future2.get();
  async_future3.get();

  auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
  std::cout << "cost ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count()
            << std::endl;

  return 0;
}
