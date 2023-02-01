/**
 * @file return_not_move.cc
 * @brief 验证：
 * 1、return obj的时候不需要move，返回值本身就是右值；见CASE： { FuncOne(DemoBuilderWithoutMove()); }, 可以编译通过
 * 2、NRVO优化，也要求return obj的时候不要move，否则性能会变差（多了一次move copy constructor）
 */
#include <iostream>
#include <string>
#include <vector>
class Demo {
 public:
  Demo() { std::cout << "constructor empty" << std::endl; }
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

void FuncOne(Demo&& demo) {
  Demo obj(std::move(demo));
  std::cout << obj.a_ << std::endl;
}

Demo DemoBuilderWithMove() {
  Demo obj(100);
  return std::move(obj);
}

Demo DemoBuilderWithoutMove() {
  Demo obj(100);
  return obj;
}

int main() {
  std::cout << "--------------return value is R value without move start------------" << std::endl;
  { FuncOne(DemoBuilderWithoutMove()); }
  std::cout << "--------------return value is R value without move end------------" << std::endl;

  std::cout << "--------------return value is R value with move start------------" << std::endl;
  { FuncOne(DemoBuilderWithMove()); }
  std::cout << "--------------return value is R value with move end------------" << std::endl;

  std::cout << "--------------return value without move start------------" << std::endl;
  {
    Demo obj(1);
    obj = DemoBuilderWithoutMove();
  }
  std::cout << "--------------return value with move end------------" << std::endl;
  {
    Demo obj(1);
    obj = DemoBuilderWithMove();
  }
  std::cout << "--------------return value with move start------------" << std::endl;

  return 0;
}

/*
--------------return value is R value without move start------------
constructor
move copy constructor
100
destructor
destructor
--------------return value is R value without move end------------
--------------return value is R value with move start------------
constructor
move copy constructor
destructor
move copy constructor
100
destructor
destructor
--------------return value is R value with move end------------
--------------return value without move start------------
constructor
constructor
move = constructor
destructor
destructor
--------------return value with move end------------
constructor
constructor
move copy constructor
destructor
move = constructor
destructor
destructor
--------------return value with move start------------
*/
