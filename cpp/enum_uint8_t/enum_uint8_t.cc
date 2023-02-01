/**
 * @file enum_uint8.cc
 * @brief 验证enum uint8_t
 */
#include <iostream>
#include <map>
#include <string>

enum class MyType : uint8_t {
  kTypeOne = 1,
  kTypeTwo = 2,
};

int main() {
  std::cout << static_cast<uint32_t>(MyType::kTypeOne) << std::endl;
  std::cout << static_cast<uint32_t>(MyType::kTypeTwo) << std::endl;

  MyType my_type = MyType::kTypeOne;
  // size为1
  std::cout << sizeof(my_type) << std::endl;
  std::cout << static_cast<uint32_t>(my_type) << std::endl;
  return 0;
}
