/**
 * @file forward_as_tuple.cc
 * @brief 测试forward_as_tuple可以减少一次临时对象的move copy。
 */
#include <iostream>
#include <map>
#include <string>
#include <tuple>

class Demo {
 public:
  Demo(const std::string& a) : a_(a) { std::cout << "constructor" << std::endl; }
  ~Demo() { std::cout << "destroy" << std::endl; }
  Demo(const Demo& obj) { std::cout << "copy constructor" << std::endl; }
  Demo(Demo&& obj) { std::cout << "move copy constructor" << std::endl; }
  Demo& operator=(const Demo& obj) {
    std::cout << "operator= constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

 private:
  std::string a_;
};

int main() {
  std::map<uint32_t, Demo> demos;

  // 相比 demos.emplace(1, Demo{"hello"}); 减少一次move copy
  demos.emplace(std::piecewise_construct, std::forward_as_tuple(1), std::forward_as_tuple("hello"));

  // C++17还可以直接这么写：
  // demos.try_emplace(1, "hello");
  return 0;
}
