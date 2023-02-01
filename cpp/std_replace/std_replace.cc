/**
 * @file std_replace.cc
 * @brief 验证：std::replace用于字符串替换，后面两个参数只能是字符，且不可以是空字符
 */
#include <algorithm>
#include <functional>
#include <iostream>

int main() {
  std::string info = "刘德华|张学友|郭富城|黎明|其他";
  std::cout << "before=" << info << std::endl;
  std::replace(info.begin(), info.end(), '|', ' ');
  std::cout << "after=" << info << std::endl;
  return 0;
}
