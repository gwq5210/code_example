/**
 * @file try_emplace.cc
 * @brief 验证：try_emplace，相比emplace，至少可以减少一次对象的move操作
 */
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Demo {
 public:
  Demo(const std::string& a) : a_(a) { std::cout << "democonstructor" << std::endl; }
  ~Demo() { std::cout << "demo destroy: a_=" << a_ << std::endl; }
  Demo(const Demo& obj) {
    std::cout << "demo copy constructor" << std::endl;
    a_ = obj.a_;
  }
  Demo(Demo&& obj) {
    std::cout << "demo move copy constructor" << std::endl;
    a_ = std::move(obj.a_);
  }
  Demo& operator=(const Demo& obj) {
    std::cout << "demo operator= constructor" << std::endl;
    a_ = obj.a_;
    return *this;
  }

 private:
  std::string a_;
};

class Demos {
 public:
  Demos(const std::vector<Demo>& demos) : demos_(demos) { std::cout << "demos constructor" << std::endl; }
  ~Demos() { std::cout << "demos destroy: demos.size()=" << demos_.size() << std::endl; }
  Demos(const Demos& obj) {
    std::cout << "demos copy constructor" << std::endl;
    demos_ = obj.demos_;
  }
  Demos(Demos&& obj) {
    std::cout << "demos move copy constructor" << std::endl;
    demos_ = std::move(obj.demos_);
  }
  Demos& operator=(const Demos& obj) {
    std::cout << "demos operator= constructor" << std::endl;
    demos_ = obj.demos_;
    return *this;
  }

 private:
  std::vector<Demo> demos_;
};

int main() {
  std::cout << "---test try_emplace(xx, Demo) start" << std::endl;
  {
    std::unordered_map<std::string, Demo> demos;
    // Demo对象实时构造
    demos.try_emplace(std::string("test"), std::string("demo"));
  }
  std::cout << "---test emplace(xx, std::move(Demo)) start" << std::endl;
  {
    std::unordered_map<std::string, Demo> demos;
    Demo obj("demo");
    demos.emplace(std::string("test"), std::move(obj));
  }

  std::cout << "---test try_emplace(xx, Demos) start" << std::endl;
  {
    std::unordered_map<std::string, Demos> demos;
    // Demos对象实时构造， 相比emplace少了一次value对象的move操作
    demos.try_emplace(std::string("test"), std::vector<Demo>{std::string("demo")});
  }
  std::cout << "---test emplace(xx, Demos) start" << std::endl;
  {
    std::unordered_map<std::string, Demos> demos;
    // 注：第二个参数不需要std::move，因为其本来就是临时生成的对象，是右值
    demos.emplace(std::string("test"), Demos({std::string("demo")}));
  }

  return 0;
}

/* 输出
 ---test try_emplace(xx, Demo) start
 democonstructor
 demo destroy: a_=demo
 ---test emplace(xx, std::move(Demo)) start
 democonstructor
 demo move copy constructor
 demo destroy: a_=
 demo destroy: a_=demo
 ---test try_emplace(xx, Demos) start
 democonstructor
 demo copy constructor
 demo copy constructor
 demos constructor
 demo destroy: a_=demo
 demo destroy: a_=demo
 demos destroy: demos.size()=1
 demo destroy: a_=demo
 ---test emplace(xx, Demos) start
 democonstructor
 demo copy constructor
 demo copy constructor
 demos constructor
 demos move copy constructor
 demos destroy: demos.size()=0
 demo destroy: a_=demo
 demo destroy: a_=demo
 demos destroy: demos.size()=1
 demo destroy: a_=demo
 * */
