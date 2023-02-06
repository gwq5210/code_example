#include <cstdio>

class Base {
 public:
  void Test() { printf("Base::Test\n"); }

 private:
  int a_ = 0;
  int b_ = 0;
};

class DeriveA : virtual public Base {
 public:
  void Print() { printf("Derive::Print\n"); }

 private:
  int e_ = 0;
};

class DeriveB : virtual public Base {
 public:
  virtual void Print() { printf("Derive::Print\n"); }

 private:
  int e_ = 0;
};

int main() {
  printf("sizeof(Base) = %zu\n", sizeof(Base));
  printf("sizeof(DeriveA) = %zu\n", sizeof(DeriveA));
  printf("sizeof(DeriveB) = %zu\n", sizeof(DeriveB));

  return 0;
}
/*
sizeof(Base) = 8
sizeof(DeriveA) = 24
sizeof(DeriveB) = 24
*/