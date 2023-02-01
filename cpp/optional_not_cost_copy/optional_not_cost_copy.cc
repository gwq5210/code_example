/**
 * @file optional_not_cost_copy.cc
 * @brief 验证返回optional<T> 是否会触发copy：返回T需要采用move，获取option中的值也需要采用move，否则都会导致对象copy
 */

#include <iostream>
#include <optional>

class Demo {
 public:
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

std::optional<Demo> GetOptionalObj(bool option) {
  if (true) {
    Demo obj(11);
    // 必须move，否则会导致copy
    return std::move(obj);
  } else {
    return std::nullopt;
  }
}

int main() {
  auto obj = GetOptionalObj(true);
  std::cout << "get obj finish ----" << std::endl;
  if (obj.has_value()) {
    // 不会触发拷贝
    // std::cout << (*obj).a_ << std::endl;

    // Demo raw_obj = *obj;  // 会导致对象拷贝
    // Demo raw_obj = std::move(*obj);  // 会调用移动拷贝构造函数
    Demo raw_obj = *(std::move(obj));  // 会调用移动拷贝构造函数
    std::cout << raw_obj.a_ << std::endl;
  }
  return 0;
}
