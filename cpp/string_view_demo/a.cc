/**
 * @file a.cc
 * @brief 测试string_view的data()是否会空字符截断：不会
 */
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

char demo[] = "1234567890";

std::string_view Get() { return std::string_view(demo, 5); }

void Write(std::string_view demo) {
  std::ofstream of("./hello");
  of << demo.data() << std::endl;  // 这里的data会被当做c格式字符串，直到空字符才停止
  of << demo.length() << std::endl;  // 这里的legnth是5
}

int main() {
  auto x = Get();
  Write(x);
  return 0;
}
