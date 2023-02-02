#include <cstdint>
#include <iostream>
#include <memory>

using namespace std;

class Base;

typedef void (*func)(Base*);
typedef void (*IntFunc)(Base*, int);

class A {
 public:
  void f() { cout << this << " A::f" << endl; }
  void g() { cout << this << " A::g" << endl; }
  void h() { cout << this << " A::h" << endl; }
};

class Base {
 public:
  virtual void f(int a) { cout << this << " Base::f: " << a << endl; }
  virtual void g() { cout << this << " Base::g" << endl; }
  virtual void h() { cout << this << " Base::h" << endl; }
};

class Derive : public Base {
 public:
  void f(int a) { cout << this << " Derive::f: " << a << endl; }
  void g() { cout << this << " Derive::g" << endl; }
  void h() { cout << this << " Derive::h" << endl; }
};

int main() {
  Base base;
  Derive derive;

  printf("base: %p\n", &base);
  printf("derive: %p\n", &derive);
  //获取vptr的地址，运行在gcc  x64环境下，所以将指针按uint64_t大小处理
  //另外基于C++的编译器应该是保证虚函数表的指针存在于对象实例中最前面的位置
  uint64_t* vPtr = (uint64_t*)(&base);
  //获取vTable 首个函数的地址
  IntFunc vTable_f = (IntFunc) * (uint64_t*)(*vPtr);
  //获取vTable 第二个函数的地址
  func vTable_g = (func) * ((uint64_t*)(*vPtr) + 1);  //加1 ，按步进计算
  func vTable_h = (func) * ((uint64_t*)(*vPtr) + 2);  //同上
  vTable_f(&base, 1);
  vTable_g(&base);
  vTable_h(&base);
  vPtr = (uint64_t*)(&derive);
  //获取vTable 首个函数的地址
  vTable_f = (IntFunc) * (uint64_t*)(*vPtr);
  //获取vTable 第二个函数的地址
  vTable_g = (func) * ((uint64_t*)(*vPtr) + 1);  //加1 ，按步进计算
  vTable_h = (func) * ((uint64_t*)(*vPtr) + 2);  //同上
  vTable_f(&derive, 1);
  vTable_g(&derive);
  vTable_h(&derive);
  cout << sizeof(A) << endl;
  cout << sizeof(base) << endl;
  cout << sizeof(derive) << endl;
  return 0;
}