#include <cstdio>

class Base {
 public:
  void Test() { printf("Base::Test\n"); }

 private:
  int a_ = 0;
  int b_ = 0;
};

class BaseB : virtual public Base {
 public:
  void TestB() { printf("BaseB::TestB\n"); }

 private:
  int c_ = 0;
};

class BaseC : virtual public Base{
 public:
  void TestC() { printf("BaseC::TestC\n"); }

 private:
  int d_ = 0;
};

class Derive : public BaseB, public BaseC {
 public:
  void Print() { printf("Derive::Print\n"); }

 private:
  int e_ = 0;
};

int main() {
  printf("sizeof(Base) = %zu\n", sizeof(Base));
  printf("sizeof(BaseB) = %zu\n", sizeof(BaseB));
  printf("sizeof(BaseC) = %zu\n", sizeof(BaseC));
  printf("sizeof(Derive) = %zu\n", sizeof(Derive));

  return 0;
}
/*
sizeof(Base) = 8
sizeof(BaseB) = 24
sizeof(BaseC) = 24
sizeof(Derive) = 40
*/