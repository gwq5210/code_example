/**
 * @file a.cc
 * @brief 验证：当结构体的成员变量都是可move类型时，编译器是否会给结构体提供默认的move构造函数. 结论：会.
 */
#include <iostream>
#include <string>
#include <vector>

class Demo {
 public:
  Demo(const std::string& a) : a_(a) { std::cout << "constructor" << std::endl; }
  ~Demo() { std::cout << "destructor" << std::endl; }
  Demo(const Demo& obj) { std::cout << "copy constructor" << std::endl; }
  Demo(Demo&& obj) { std::cout << "move copy constructor" << std::endl; }
  Demo& operator=(Demo&& obj) { std::cout << "move operator=" << std::endl; }
  Demo& operator=(const Demo& obj) { std::cout << "operator=" << std::endl; }

 private:
  std::string a_;
};

struct Final {
  std::vector<Demo> objs;
};

int main() {
  Final obj_a;
  obj_a.objs.push_back(Demo("hello world"));
  obj_a.objs.push_back(Demo("hello world2"));
  std::cout << "----" << std::endl;
  // Final默认产生move 构造函数
  Final obj_b = std::move(obj_a);
  return 0;
}
