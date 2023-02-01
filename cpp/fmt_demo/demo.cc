/**
 * @file demo.cc
 * @brief 测试fmt api的使用
 */

#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>

void JoinFloatWithPrecision() {
  std::vector<float> values = {1.33432432, 2.121, 3.34};
  std::string demo = fmt::format("{:.4f}", fmt::join(values, ","));
  std::cout << demo << std::endl;
}

void NotGootWayToJon() {
  std::string demo;
  std::vector<float> values = {1.33432432, 2.121, 3.34};
  for (size_t i = 0; i < values.size() - 1; ++i) {
    demo += fmt::sprintf("%.4f,", values[i]);
  }
  demo += fmt::sprintf("%.4f", values[values.size() - 1]);
  std::cout << demo << std::endl;
}

int main() {
  JoinFloatWithPrecision();
  NotGootWayToJon();
  return 0;
}

/*
1.3343, 2.1210, 3.3400
*/