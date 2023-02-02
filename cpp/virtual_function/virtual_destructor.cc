#include <iostream>
#include <string>

class VBase {
 public:
  VBase() { printf("VBase::VBase\n"); }

  virtual ~VBase() { printf("VBase::~VBase\n"); }

  virtual void func1() { printf("VBase::func1\n"); }
};

class Base {
 public:
  Base() { printf("Base::Base\n"); }

  ~Base() { printf("Base::~Base\n"); }

  virtual void func1() { printf("Base::func1\n"); }
};

class VDerive : public VBase {
 public:
  VDerive() { printf("VDerive::VDerive\n"); }
  ~VDerive() { printf("VDerive::~VDerive\n"); }
  void func1() override { printf("VDerive::func1\n"); }
};

class Derive : public Base {
 public:
  Derive() { printf("Derive::Derive\n"); }
  ~Derive() { printf("Derive::~Derive\n"); }
  void func1() override { printf("Derive::func1\n"); }
};

int main(int argc, char* argv[]) {
  {
    printf("----\n");
    Derive* d = new Derive();
    d->func1();
    delete d;
    printf("----\n");
  }

  {
    printf("----\n");
    Base* b = new Derive();
    b->func1();
    delete b;
    printf("----\n");
  }

  {
    printf("----\n");
    VDerive* vd = new VDerive();
    vd->func1();
    delete vd;
    printf("----\n");
  }

  {
    printf("----\n");
    VBase* vb = new VDerive();
    vb->func1();
    delete vb;
    printf("----\n");
  }
  return 0;
}