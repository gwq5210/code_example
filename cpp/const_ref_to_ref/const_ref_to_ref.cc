/**
 * @file const_ref_to_ref.cc
 * @brief 验证用&去接const&不会导致拷贝
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

std::map<uint32_t, Demo> demos;

const Demo& Test() { return demos.at(1); }

int main() {
  demos.emplace(std::piecewise_construct, std::forward_as_tuple(1), std::forward_as_tuple("hello"));
  // 以下带有&的都不会导致拷贝
  // const auto& x = Test();
  // auto& x = Test();

  // 不带有&的会导致拷贝
  auto x = Test();
  return 0;
}
