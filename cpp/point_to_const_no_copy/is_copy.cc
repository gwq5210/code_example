/**
 * @file is_copy.cc
 * @brief const引用接手*指针，不会引发拷贝
 */
#include <iostream>
#include <map>
#include <string>

class Demo {
 public:
  Demo() { std::cout << "constr" << std::endl; }
  Demo(const Demo& obj) { std::cout << "copy constr" << std::endl; }
};

void Test(std::map<std::string, Demo>* xx) {
  // 这里不会引发拷贝
  const auto& x = *xx;
  std::cout << x.begin()->first << std::endl;
}

int main() {
  std::map<std::string, Demo> map_obj;
  map_obj.insert({"hello", Demo()});
  std::cout << "--------" << std::endl;
  Test(&map_obj);
  std::cout << "--------" << std::endl;
  return 0;
}
