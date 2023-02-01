/**
 * @file optional_value_demo.cc
 * @brief 测试 yaml-cpp 接口的使用
 */

#include <iostream>
#include <optional>
#include <string>

class Demo {
 public:
  Demo(int a) : a_(a) { std::cout << "constructor" << std::endl; }
  Demo(const Demo& obj) {
    std::cout << "copy constructor" << std::endl;
    a_ = obj.a_;
  }
  Demo& operator=(const Demo& obj) {
    std::cout << "= copy constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

  Demo(Demo&& obj) {
    std::cout << "move copy constructor" << std::endl;
    a_ = obj.a_;
  }
  Demo& operator=(Demo&& obj) {
    std::cout << "move = constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

  ~Demo() { std::cout << "destructor" << std::endl; }

 public:
  int a_ = 0;
};

void TestDemo(Demo* obj) { std::cout << obj->a_ << std::endl; }

int main() {
  std::optional<Demo> op_demo;
  op_demo.emplace(1);
  {
    std::cout << "---start----" << std::endl;
    std::cout << op_demo.value().a_ << std::endl;
    std::cout << "---end----" << std::endl;
  }
  { TestDemo(op_demo); }

  return 0;
}
