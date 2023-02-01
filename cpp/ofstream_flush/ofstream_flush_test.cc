/**
 * @file a.cc
 * @brief
 */
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

void RunWithOutFlush() {
  std::ofstream of("hello1");
  for (uint32_t i = 0; i != 200000; ++i) {
    of.write("1234567", 7);
  }
}

void RunWithFlush() {
  std::ofstream of("hello2");
  for (uint32_t i = 0; i != 200000; ++i) {
    of.write("1234567", 7);
    of.flush();
  }
}

void RunWithFlushAndSkip() {
  std::ofstream of("hello3");
  for (uint32_t i = 0; i != 200000; ++i) {
    of.seekp(of.tellp());
    of.write("1234567", 7);
    of.flush();
  }
}

int main() {
  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithOutFlush();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithOutFlush cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithFlush();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithFlush cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithFlushAndSkip();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithFlushAndSkip cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithOutFlush();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithOutFlush cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithFlush();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithFlush cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    RunWithFlushAndSkip();
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << "RunWithFlushAndSkip cost_ms:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count() << std::endl;
  }

  return 0;
}
