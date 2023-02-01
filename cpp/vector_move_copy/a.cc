/**
 * @file a.cc
 * @brief 测试怎么高效率的做两个vector的合并
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

int main() {
  std::vector<Demo> input{Demo("hello"), Demo("world")};
  std::cout << "--------" << std::endl;
  std::vector<Demo> dest;
  // 这个不会导致拷贝，调用的是move operator=
  // dest.insert(dest.end(), std::make_move_iterator(input.begin()), std::make_move_iterator(input.end()));
  // 下面会导致拷贝
  dest.insert(dest.end(), input.begin(), input.end());
  std::cout << "input:" << input.size() << std::endl;
  return 0;
}
