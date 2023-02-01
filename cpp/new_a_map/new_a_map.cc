/**
 * @file new_a_map.cc
 * @brief 验证：map的默认构造函数
 */
#include <iostream>
#include <map>
#include <string>

void Copy(std ::map<std::string, std::string>* demo) {
  std::map<std::string, std::string>* demo2 = new std::map<std::string, std::string>(*demo);
  std::cout << demo2->size() << std::endl;
}

int main() {
  std::map<std::string, std::string> demo{{"1", "2"}, {"x", "y"}};

  Copy(&demo);

  return 0;
}
