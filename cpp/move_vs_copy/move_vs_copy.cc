/**
 * @file move_vs_copy
 * @brief 测试验证怎么写出来不引发copy的函数及其调用
 */
#include <iostream>
#include <string>
#include <vector>

class Demo {
 public:
  Demo() { std::cout << "empty constructor" << std::endl; }
  Demo(const std::string& a) : a_(a) { std::cout << "constructor" << std::endl; }
  ~Demo() { std::cout << "destroy" << std::endl; }
  Demo(const Demo& obj) { std::cout << "copy constructor" << std::endl; }
  Demo(Demo&& obj) { std::cout << "move copy constructor" << std::endl; }
  Demo& operator=(const Demo& obj) {
    std::cout << "operator= constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }
  Demo& operator=(Demo&& obj) {
    std::cout << "move operator = constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

 private:
  std::string a_;
};

class Owner {
 public:
  Owner() {}

  // 这里如果加上const会导致copy
  void TestMove(std::vector<Demo>&& demo) { demos_ = std::move(demo); }

  // 这里的效果和&&一样，但调用者不需要显式的std::move()，可读性比较差
  void TestMove(std::vector<Demo>& demo) { demos_ = std::move(demo); }

  // 这里的move没有效果，会导致demo对象赋值拷贝
  void TestMoveConstItem(const std::vector<Demo>& demo) { fake_ = std::move(demo[0]); }

  // 这里的move没有效果，会导致demo对象赋值拷贝
  void TestMoveConst(const std::vector<Demo>& demo) { demos_ = std::move(demo); }

 private:
  std::vector<Demo> demos_;
  Demo fake_;
};

int main() {
  std::vector<Demo> demos;
  demos.emplace_back("hello");
  std::cout << "-------" << std::endl;
  Owner obj;
  // obj.TestMove(std::move(demos));
  // obj.TestMove(demos);
  obj.TestMoveConstItem(demos);

  std::cout << "-------" << std::endl;
  return 0;
}
