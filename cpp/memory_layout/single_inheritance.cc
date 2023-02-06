#include <cstdio>

class Base {
 public:
  virtual ~Base() {}
  virtual void Test() { printf("Base::Test\n"); }

 private:
  int a_ = 0;
  int b_ = 0;
};

class Derive : public Base {
 public:
  virtual ~Derive() {}
  virtual void Test() override { printf("Derive::Test\n"); }
  virtual void Print() { printf("Derive::Print\n"); }

 private:
  int c_ = 0;
};

int main() {
  Base base;
  Derive derive;

  printf("sizeof(Base) = %zu\n", sizeof(Base));
  printf("sizeof(Derive) = %zu\n", sizeof(Derive));

  return 0;
}
/*
sizeof(Base) = 16
sizeof(Derive) = 24
*/