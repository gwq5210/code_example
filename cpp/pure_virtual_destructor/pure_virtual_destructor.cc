#include <iostream>
#include <string>

class Base {
 public:
  Base() { std::cout << "Base::Base" << std::endl; }
  virtual void func1() = 0;

  // 错误，不可以这样写
  // virtual void func1() = 0 {
  //   std::cout << "Base::func1" << std::endl;
  // }

  virtual ~Base() = 0;  // 纯虚函数
};

// 可以提供纯虚函数的函数体，但是永远也不会被调用
void Base::func1() {  // 纯虚函数func1 函数体
  std::cout << "Base::func1" << std::endl;
}

Base::~Base() {  // 纯虚析构函数 函数体
  std::cout << "Base::~Base" << std::endl;
}

class Derive : public Base {
 public:
  void func1() override { std::cout << "Derive::func1" << std::endl; }
  Derive() { std::cout << "Derive::Derive" << std::endl; }
  ~Derive() { std::cout << "Derive::~Derive" << std::endl; }
};

int main(int argc, char* argv[]) {
  Derive d;
  Base* b = &d;
  b->func1();
  return 0;
}
