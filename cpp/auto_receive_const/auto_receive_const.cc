/**
 * @file auto_receive_const.cc
 * @brief 当用auto去接一个const引用时，是否会引发const引用对象的拷贝？会！
 */

#include <iostream>
#include <string>
#include <vector>

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

class Owner {
 public:
  const Demo& RetDemoConst() { return demo_; }

 private:
  Demo demo_{"刘德华"};
};

int main() {
  Owner owner;
  std::cout << "-------start auto x= -------" << std::endl;
  // 下面会触发Demo对象的拷贝
  auto x = owner.RetDemoConst();
  std::cout << "-------end auto x= -------" << std::endl;
  // 下面不会触发Demo对象的拷贝
  std::cout << "-------start const auto& xx= -------" << std::endl;
  const auto& xx = owner.RetDemoConst();
  std::cout << "-------end const auto& xx= -------" << std::endl;
  return 0;
}

/*
 constructor
-------start auto x= -------
copy constructor
-------end auto x= -------
-------start const auto& xx= -------
-------end const auto& xx= -------
destroy
destroy
 */
