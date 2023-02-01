/**
 * @file for_range.cc
 * @brief 验证：for range中的函数调用，只会调用一次
 */
#include <iostream>
#include <map>
#include <string>

class Demo {
 public:
  Demo(int a) : a_(a) { std::cout << "Constructor" << std::endl; }
  ~Demo() { std::cout << "Destructor" << std::endl; }
  Demo(const Demo& obj) { std::cout << "Copy Constructor" << std::endl; }

 private:
  int a_;
};

class DemoOwner {
 public:
  DemoOwner() {
    demos_.emplace(1, Demo{1});
    demos_.emplace(2, Demo{2});
    demos_.emplace(3, Demo{3});
  }

  // 如果返回非const& 会触发拷贝
  const std::map<uint32_t, Demo>& GetDemos() { return demos_; }

 private:
  std::map<uint32_t, Demo> demos_;
};

int main() {
  DemoOwner owner;
  std::cout << "----------" << std::endl;
  // owner.GetDemos()在整个for循环过程中，只会调用一次
  for (const auto& kv : owner.GetDemos()) {
    std::cout << kv.first << std::endl;
  }
  std::cout << "----------" << std::endl;
  return 0;
}
