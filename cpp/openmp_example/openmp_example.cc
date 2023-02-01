/**
 * @file openmp_example.cc
 * @brief openmp 例子
 * @note 参考：https://blog.csdn.net/zhongkejingwang/article/details/40350027
 */
#include <iostream>
#include <thread>
#include <unordered_map>

#include "omp.h"

void SimpleExample() {
  uint32_t i = 0;
  omp_set_num_threads(4);
#pragma omp parallel
  {
    std::cerr << "thread num:" << omp_get_thread_num() << ", thread_id=" << std::this_thread::get_id() << std::endl;
    std::cerr << "do job, i=" << i++ << std::endl;
  }
}

void ForExample() {
  // 2线程并发，for循环
  omp_set_num_threads(2);
#pragma omp parallel for
  for (auto i = 0; i < 4; ++i) {
    std::cerr << "thread num:" << omp_get_thread_num() << std::endl;
  }
}

void ForExample2() {
  // 2线程并发，for循环
#pragma omp parallel num_threads(2)
  {
#pragma omp for
    for (auto i = 0; i < 4; ++i) {
      std::cerr << "thread num:" << omp_get_thread_num() << std::endl;
    }
  }
}

void ForExampleCritical() {
  std::unordered_map<std::string, std::string> kv;
  uint32_t sum = 0;

  // openmp使用8个线程
  omp_set_num_threads(8);

// for循环并发执行，一个线程一个任务，顺序分配，但执行不一定是顺序的
#pragma omp parallel for schedule(static, 1)
  for (auto i = 0; i < 100000; ++i) {
    std::cout << i << std::endl;
    std::string key = std::to_string(i);

// 设置为竞争资源，确保openmp的多线程安全
#pragma omp critical
    {
      // kv非多线程安全
      kv.emplace(key, key);
    }
  }

  std::cout << "kv.size=" << kv.size() << std::endl;
}

void ForExampleCritical2() {
  std::unordered_map<std::string, std::string> kv;
  uint32_t sum = 0;

  // openmp使用8个线程
  omp_set_num_threads(8);

// for循环并发执行，部分需要按串行顺序执行
#pragma omp parallel for schedule(dynamic) ordered
  for (auto i = 0; i < 100000; ++i) {
    std::cout << i << std::endl;
    std::string key = std::to_string(i);

// 设置为按串行的迭代顺序执行
#pragma omp ordered
    {
      // kv非多线程安全
      kv.emplace(key, key);
    }
  }

  std::cout << "kv.size=" << kv.size() << std::endl;
}

int main() {
  SimpleExample();
  std::cout << "-------" << std::endl;
  ForExample();
  std::cout << "-------" << std::endl;
  ForExample2();
  std::cout << "-------" << std::endl;
  ForExampleCritical2();
  return 0;
}

/*
thread num:0, thread_id=139895877713024
do job, i=0
thread num:1, thread_id=139895856375552
do job, i=1
thread num:3, thread_id=139895839590144
do job, i=2
thread num:2, thread_id=139895847982848
do job, i=3
-------
thread num:0
thread num:0
thread num:1
thread num:1
-------
thread num:thread num:1
thread num:1
0
thread num:0
-------
kv.size=100000
*/
