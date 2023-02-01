/**
 * @file emplace_back.cc
 * @brief 对比emplace_back在给出类构造参数、给出类对象下的差异
 * 结果：
 * (1)正确的emplace_back会比push_back少一次move
 * (2)假的emplace_back性能和push_back一样
 *
 * 参考：https://en.cppreference.com/w/cpp/container/vector/emplace_back
 */
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

struct President {
  std::string name;
  std::string country;
  int year;

  President(std::string p_name, std::string p_country, int p_year)
      : name(std::move(p_name)), country(std::move(p_country)), year(p_year) {
    std::cout << "I am being constructed.\n";
  }
  President(President&& other) : name(std::move(other.name)), country(std::move(other.country)), year(other.year) {
    std::cout << "I am being moved.\n";
  }
  President& operator=(const President& other) = default;
};

int main() {
  std::vector<President> elections;
  std::cout << "emplace_back:\n";
  auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
  assert(ref.year == 1994 && "uses a reference to the created object (C++17)");

  std::vector<President> reElections;
  std::cout << "\npush_back:\n";
  reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

  std::vector<President> elections2;
  std::cout << "\nfake emplace_back:\n";
  auto& ref2 = elections2.emplace_back(President("Nelson Mandela", "South Africa", 1994));
  assert(ref2.year == 1994 && "uses a reference to the created object (C++17)");

  std::cout << "\nContents:\n";
  for (President const& president : elections) {
    std::cout << president.name << " was elected president of " << president.country << " in " << president.year
              << ".\n";
  }
  for (President const& president : reElections) {
    std::cout << president.name << " was re-elected president of " << president.country << " in " << president.year
              << ".\n";
  }
}
