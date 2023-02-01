/**
 * @file move_call_copy.cc
 * @brief 验证：移动赋值函数不能直接调用移动拷贝构造函数实现，如果有公共的部分，需要封装公共函数
 */
#include <iostream>
#include <string>
#include <vector>

class MyClass {
 public:
  MyClass() {
    std::cout << "MyClass constructor" << std::endl;
    BuildData();
    data_[0] = 1;
  }
  ~MyClass() { ReleaseData(); }

  MyClass(const MyClass& obj) {
    std::cout << "MyClass copy constructor" << std::endl;
    BuildData();
    data_[0] = obj.data_[0];
  }

  MyClass& operator=(const MyClass& obj) {
    std::cout << "MyClass operator= constructor" << std::endl;
    if (this == &obj) {
      return *this;
    }

    // 以下实现仅是常用法示例，实际上本case可以复用data_的空间
    // 释放掉老的
    ReleaseData();

    // 再申请空间
    BuildData();

    data_[0] = obj.data_[0];
    return *this;
  }

  MyClass(MyClass&& obj) {
    std::cout << "MyClass move copy constructor" << std::endl;
    MoveData(std::move(obj));
  }

  MyClass& operator=(MyClass&& obj) {
    std::cout << "MyClass move operator= constructor" << std::endl;
    if (this == &obj) {
      return *this;
    }

    ReleaseData();

    MoveData(std::move(obj));
    // 注意：不能通过移动拷贝构造函数来实现功能
    // MyClass(std::move(obj)); // 这行代码只会导致构造临时对象

    return *this;
  }

  void MoveData(MyClass&& obj) {
    data_ = obj.data_;
    obj.data_ = nullptr;
  }

  void BuildData() { data_ = new int[100]; }

  void ReleaseData() {
    if (data_ != nullptr) {
      delete[] data_;
      data_ = nullptr;
    }
  }

  int* data_ = nullptr;
};

int main() {
  MyClass obj1;
  MyClass obj2;
  std::cout << "obj1.data_=" << obj1.data_ << std::endl;
  std::cout << "obj2.data_=" << obj2.data_ << std::endl;

  obj2 = std::move(obj1);
  std::cout << "obj1.data_=" << obj1.data_ << std::endl;
  std::cout << "obj2.data_=" << obj2.data_ << std::endl;

  return 0;
}
