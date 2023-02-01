/**
 * @file nrvo_example.cc
 * @brief 函数返回的对象，可以不触发拷贝，术语叫做NRVO。
 * 本demo的目的：
 * 1、验证NRVO的价值。
 * 2、验证当不采用NRVO而使用std::move时，需要付出移动拷贝构造的代价，建议不要用move。
 * 3、验证函数的返回值含有不带名字的返回值分支时，NRVO会失效，转为move。
 */

#include <iostream>
#include <string>

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

Demo GetObjectWithNRVO() {
  Demo obj(10);
  obj.a_ -= 1;
  std::cout << "----GetObjectWithNRVO() return----" << std::endl;
  return obj;

  // 如果返回的不是一个命名对象，而是一个需要隐式构造对象的数值，则不触发NRVO优化，建议对象不要支持隐式类型转换，以免误触。
  // return 10;
}

Demo GetObjectWithMove() {
  Demo obj(10);
  obj.a_ -= 1;
  std::cout << "----GetObjectWithMove() return----" << std::endl;
  // 不建议在返回时用move，因为相比NRVO多了一次移动构造函数的调用
  return std::move(obj);
}

Demo GetObjectWithoutNRVO(bool flag = false) {
  if (flag) {
    return 10;
  }
  Demo obj(10);
  obj.a_ -= 1;
  std::cout << "----GetObjectWithoutNRVO() return----" << std::endl;
  return obj;
}

struct Wrapper {
  Demo obj1;
  Demo obj2;
};

int main() {
  {
    std::cout << "----test NRVO wrapper----" << std::endl;
    Wrapper wrapper;
    // 这里的赋值操作会调用 move = constructor，不需要显示的加上std::move
    wrapper.obj1 = GetObjectWithNRVO();
    std::cout << "----test NRVO wrapper end----" << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl;

  {
    std::cout << "----test NRVO----" << std::endl;
    // 这里的赋值操作会使用NRVO
    Demo ret_obj = GetObjectWithNRVO();
    std::cout << "----test NRVO end----" << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl;

  {
    std::cout << "----test move----" << std::endl;
    Demo ret_obj = GetObjectWithMove();
    std::cout << "----test move end----" << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl;

  /*

  {
    std::cout << "----test NOT name return----" << std::endl;
    Demo ret_obj = GetObjectWithoutNRVO();
    std::cout << "----test NOT name return end----" << std::endl;
  }*/

  return 0;
}

/*
----test NRVO----
constructor
----GetObjectWithNRVO() return----
----test NRVO end----
destructor



----test move----
constructor
----GetObjectWithMove() return----
move copy constructor
destructor
----test move end----
destructor



----test NOT name return----
constructor
----GetObjectWithoutNRVO() return----
move copy constructor
destructor
----test NOT name return end----
destructor

*/
