/**
 * @file vector_initialize_list.cc
 * @brief 验证：vector的初始化列表({item})，相比emplace_back(move)会多一次对象拷贝
 */
#include <iostream>
#include <string>
#include <vector>

class Demo {
 public:
  Demo(const std::string& a) : a_(a) { std::cout << "constructor" << std::endl; }
  ~Demo() { std::cout << "destroy: a_=" << a_ << std::endl; }
  Demo(const Demo& obj) {
    std::cout << "copy constructor" << std::endl;
    a_ = obj.a_;
  }
  Demo(Demo&& obj) {
    std::cout << "move copy constructor" << std::endl;
    a_ = std::move(obj.a_);
  }
  Demo& operator=(const Demo& obj) {
    std::cout << "operator= constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

 private:
  std::string a_;
};

int main() {
  {
    Demo obj("hello world");
    std::cout << "====one move:" << std::endl;
    std::vector<Demo> xx;
    xx.emplace_back(std::move(obj));
    std::cout << "====end" << std::endl;
  }
  {
    Demo obj("hello world");
    std::cout << "====one move and one copy:" << std::endl;
    std::vector<Demo> zz{std::move(obj)};
    std::cout << "====end" << std::endl;
  }
  return 0;
}
/* 输出
 constructor
 ====one move:
 move copy constructor
 ====end
destroy: a_=hello world
destroy: a_=
constructor
====one move and one copy:
move copy constructor
copy constructor
destroy: a_=hello world
====end
destroy: a_=hello world
destroy: a_=
 * */
