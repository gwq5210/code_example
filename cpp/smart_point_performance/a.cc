/**
 * @file a.cc
 * @brief 智能指针频繁访问时的性能
 * 测试数据： 裸指针 > 共享智能指针 > 独占智能指针
 * unique cost_ms:3027
 * shared cost_ms:1856
 * ram cost_ms:1324
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

class Demo {
 public:
  std::string demo_ = "hello world";
  int a_ = 1;
};

int main() {
  // std::string tmp;
  int b = 0;
  auto unique_demo = std::make_unique<Demo>();
  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    for (uint32_t i = 0; i != 100000000; ++i) {
      b = unique_demo->a_;
      // tmp = unique_demo->demo_;
    }
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "unique cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  auto shared_demo = std::make_shared<Demo>();
  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    for (uint32_t i = 0; i != 100000000; ++i) {
      b = shared_demo->a_;
      // tmp = shared_demo->demo_;
    }
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "shared cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  auto ram_demo = new Demo();
  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    for (uint32_t i = 0; i != 100000000; ++i) {
      b = ram_demo->a_;
      // tmp = ram_demo->demo_;
    }
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "ram cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }
  return 0;
}
