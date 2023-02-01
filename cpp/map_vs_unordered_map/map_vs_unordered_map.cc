/**
 * @file map_vs_unordered_map.cc
 * @brief 验证map和unordered_map的性能对比：
 * （1）在100~亿级别数据量下unordered_map查找性能更好；
 * （2）在大数据量下map的emplace性能更好，但如果unordered_map可以先reserve空间，则unordered_map emplace性能更好；
 */
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

std::set<std::string> ReleaseRandomSet(std::size_t element_size) {
  std::set<std::string> elements;
  while (elements.size() < element_size) {
    // 限定随机数范围为：0 ~ 元素个数的两倍
    elements.insert(std::to_string(rand() % (element_size * 2)));
  }
  return elements;
}

template <typename KvType>
void TestMapTemplate(const std::set<std::string>& elements, const std::set<std::string>& find_elements,
                     std::size_t find_count) {
  KvType kv;
  {
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    for (const auto& item : elements) {
      kv.emplace(item, 1);
    }
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << std::string(typeid(kv).name()).substr(0, 13) << "|insert element size=" << elements.size()
              << " cost_ms:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count()
              << std::endl;
  }

  {
    uint32_t find_match = 0;
    auto start_commit = std::chrono::steady_clock::now().time_since_epoch();
    auto it = find_elements.begin();
    for (std::size_t i = 0; i != find_count; ++i, ++it) {
      if (it == find_elements.end()) {
        it = find_elements.begin();
      }
      auto itt = kv.find(*it);
      if (itt != kv.end()) {
        ++find_match;
      }
    }
    auto end_commit = std::chrono::steady_clock::now().time_since_epoch();
    std::cout << std::string(typeid(kv).name()).substr(0, 13) << "|element size=" << elements.size()
              << "|find count=" << find_count << "|find match=" << find_match
              << " cost_ms:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_commit - start_commit).count()
              << std::endl;
  }
}

void TestMap(std::size_t element_size, std::size_t find_count) {
  auto kv_elements = ReleaseRandomSet(element_size);
  // 查找的元素个数设置为被查找元素的一半，用于验证找到元素只有1半时的性能
  auto find_elements = ReleaseRandomSet(element_size / 2);

  /*
  std::ofstream kv_elements_out("kv_elements");
  for (const auto& item : kv_elements) {
    kv_elements_out << item << std::endl;
  }
  std::ofstream find_elements_out("find_elements");
  for (const auto& item : find_elements) {
    find_elements_out << item << std::endl;
  }*/

  TestMapTemplate<std::map<std::string, int>>(kv_elements, find_elements, find_count);
  TestMapTemplate<std::unordered_map<std::string, int>>(kv_elements, find_elements, find_count);
}

int main() {
  srand((int)time(0));
  TestMap(10, 1000000);
  TestMap(100, 1000000);
  TestMap(1000, 1000000);
  TestMap(10000, 1000000);
  TestMap(100000, 1000000);
  TestMap(1000000, 1000000);
  TestMap(10000000, 1000000);
  TestMap(100000000, 1000000);
  return 0;
}
