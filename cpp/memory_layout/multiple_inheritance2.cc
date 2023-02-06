#include <cstdio>

class BaseA {
 public:
  void TestA() { printf("BaseA::TestA\n"); }

 private:
  int a_ = 0;
  int b_ = 0;
};

class BaseB {
 public:
  virtual ~BaseB() {}
  virtual void TestB() { printf("BaseB::TestB\n"); }

 private:
  int c_ = 0;
};

class Derive : public BaseA, public BaseB {
 public:
  virtual ~Derive() {}
  virtual void TestB() override { printf("Derive::TestB\n"); }
  virtual void Print() { printf("Derive::Print\n"); }

 private:
  int d_ = 0;
};

int main() {
  BaseA base_a;
  BaseB base_b;
  Derive derive;

  printf("sizeof(BaseA) = %zu\n", sizeof(BaseA));
  printf("sizeof(BaseB) = %zu\n", sizeof(BaseB));
  printf("sizeof(Derive) = %zu\n", sizeof(Derive));

  return 0;
}
/*
sizeof(BaseA) = 8
sizeof(BaseB) = 16
sizeof(Derive) = 24
*/