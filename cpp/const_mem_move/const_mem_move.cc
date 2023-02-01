/**
 * @file const_mem_move.cc
 * @brief 验证：const对象的成员或者成员的成员进行move，不会调用移动构造，而是直接拷贝
 */
#include <iostream>
#include <map>
#include <string>

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

struct Owner {
  std::map<std::string, Demo> infos;
};

// 测试const对象的一级成员move
void TestConstMemMove(const Owner& obj) {
  // 这里的move会变成拷贝
  std::map<std::string, Demo> infos = std::move(obj.infos);
}

// 测试const对象的二级成员move
void TestConstMemMemMove(const Owner& obj) {
  for (auto&& [key, value] : obj.infos) {
    // 这里的move会变成拷贝
    auto demo = std::move(value);
  }
}

// 万能引用对象的二级成员move
void TestConstMemMemMove(Owner&& obj) {
  for (auto&& [key, value] : obj.infos) {
    // 这里的move是真move
    auto demo = std::move(value);
  }
}

int main() {
  {
    Owner obj;
    obj.infos.try_emplace("key", "values");
    std::cout << "----start const 的一级成员move test----" << std::endl;
    TestConstMemMove(obj);
    std::cout << "----end const 的一级成员move test----" << std::endl;
  }

  {
    Owner obj;
    obj.infos.try_emplace("key", "values");
    std::cout << "----start const 的二级级成员move test----" << std::endl;
    TestConstMemMemMove(obj);
    std::cout << "----end const 的二级成员move test----" << std::endl;
  }

  {
    Owner obj;
    obj.infos.try_emplace("key", "values");
    std::cout << "----start 万能引用的二级级成员move test----" << std::endl;
    TestConstMemMemMove(std::move(obj));
    std::cout << "----end 万能引用的二级级成员move test----" << std::endl;
  }

  return 0;
}
