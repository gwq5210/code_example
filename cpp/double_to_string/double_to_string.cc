/**
 * @file double_to_string.cc
 * @brief double到string的转换，去掉末尾的0
 */
#include <algorithm>
#include <iostream>
#include <string>

std::string DoubleToString(double num) {
  std::string str = std::to_string(num);
  std::cout << "to_string=" << str << std::endl;
  if (str.find('.') != std::string::npos) {
    // 删除末尾的无效0
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return ch != '0'; }).base(), str.end());
    std::cout << "delete end 0=" << str << std::endl;
    // 删除末尾的无效.
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return ch != '.'; }).base(), str.end());
    std::cout << "delete end .=" << str << std::endl;
  }
  return str;
}

int main() {
  double num = 1636419632;
  std::cout << std::to_string(num) << std::endl;
  std::cout << DoubleToString(num) << std::endl;
  return 0;
}
