/**
 * @file move_within_sub_func
 * @brief 当子函数返回&&，当前函数将其右值再次返回时，不会触发拷贝
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
  Demo&& MoveIt() { return std::move(demo_); }

 private:
  Demo demo_{"刘德华"};
};

// 这里返回Demo不会触发从右值到左值的对象拷贝
Demo GetIt(Owner* obj) { return obj->MoveIt(); }

int main() {
  Owner owner;
  std::cout << "-------start GetIt-------" << std::endl;
  auto x = GetIt(&owner);
  std::cout << "-------end GetIt-------" << std::endl;
  return 0;
}

/*
constructor
-------start GetIt-------
move copy constructor
destroy
-------end GetIt-------
destroy
*/
