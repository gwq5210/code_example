/**
 * @file unique_handler.h
 * @brief 比unique_ptr多了无所有权拷贝的智能指针。
 * 业务背景：
 * 1、SKit的计算层需要从内存库和磁盘库中获取正排索引数据，计算层无法得知具体的库是什么类型（库是多态设计）；
 * 2、当从内存库中获取时，资源所有权归内存库所有；当从磁盘库获取时(实时从磁盘中读取)，资源所有权归调用者；
 * 3、因为必须统一接口，且所有权归属不固定属于调用或者被调方，使得必须返回一个带有生命周期控制的封装，即是智能指针；
 * 4、当采用std::shared_ptr时，智能指针对象的拷贝有底层锁去保证引用计数原子加减，当高频发生时，有较高的时间消耗；
 * 5、本场景只需要带有可选释放指针的封装类，不需要有引用计数型智能指针这么重的设计；
 * 技术分析：
 * 1、诉求：能支持类似智能指针对象的拷贝传递，但又可以按需设置指针由哪方做释放，且耗时较短。
 * 2、已有方案：unique_ptr，可以确保只有一个对象拥有资源管理权，但不支持拷贝；
 *    shared_ptr支持拷贝，也可使资源被调用方或者被调方释放，但需要锁确保多线程的引用计数准确，使得耗时较高。
 * 3、本代码方案：实现一个资源管理器类似unique_ptr，但比unique_ptr增加无所有权的拷贝。
 */
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief 支持无所有权拷贝的唯一智能指针
 * @tparam T 指针指向的类型
 */
template <typename T>
class UniqueHandler {
 public:
  UniqueHandler() = default;
  explicit UniqueHandler(T* item, bool is_owner = true) noexcept : item_(item), is_owner_(is_owner) {}

  /// 禁止拷贝，如需要拷贝或者所有权转移可通过Shadow()或者move来做
  UniqueHandler(const UniqueHandler&) = delete;

  UniqueHandler(UniqueHandler&& other) noexcept {
    item_ = other.item_;
    is_owner_ = other.is_owner_;
    other.item_ = nullptr;
    other.is_owner_ = false;
  }

  /// 禁止赋值，如需要拷贝或者所有权转移可通过Shadow()或者move来做
  UniqueHandler& operator=(const UniqueHandler&) = delete;

  UniqueHandler& operator=(UniqueHandler&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (is_owner_) {
      delete item_;
    }
    item_ = other.item_;
    is_owner_ = other.is_owner_;
    other.item_ = nullptr;
    other.is_owner_ = false;
    return *this;
  }

  ~UniqueHandler() noexcept {
    if (!is_owner_) {
      return;
    }
    delete item_;
  }

  T* Get() const noexcept { return item_; }

  T* operator->() const noexcept { return item_; }

  T& operator*() const noexcept { return *item_; }

  UniqueHandler& operator=(std::nullptr_t) noexcept {
    Reset();
    return *this;
  }

  operator bool() const noexcept { return item_ != nullptr; }

  bool operator==(std::nullptr_t) const noexcept { return item_ == nullptr; }

  bool operator!=(std::nullptr_t) const noexcept { return item_ != nullptr; }

  /// 重置被管理的指针，重置后带有owner属性
  void Reset(T* new_item = nullptr) noexcept {
    if (is_owner_) {
      delete item_;
    }
    item_ = new_item;
    is_owner_ = true;
  }

  // 注：不考虑支持类似unique_ptr的Release函数，
  // 因为Release函数会返回指针以及放弃本handler对象的owner属性，
  // 调用者无法得知在他调用之前，本handler对象是否是指针的owner，因此无法决策后续他是否需要delete拿到的指针。

  /// 无所有权拷贝，不带有owner属性
  UniqueHandler Shadow() const noexcept {
    UniqueHandler obj(item_, false);
    return obj;
  }

 private:
  T* item_ = nullptr;
  bool is_owner_ = true;
};

/// Demo类
class Demo {
 public:
  Demo(const std::string& a) : a_(a) { std::cout << "constructor demo, a=" << a << std::endl; }

  Demo(const Demo& obj) {
    std::cout << "copy constructor demo, obj.a_=" << obj.a_ << std::endl;
    a_ = obj.a_;
  }

  Demo(Demo&& obj) {
    std::cout << "move copy constructor demo, obj.a_=" << obj.a_ << std::endl;
    a_ = std::move(obj.a_);
  }

  Demo& operator=(const Demo& obj) {
    std::cout << "= copy constructor demo, obj.a_=" << obj.a_ << std::endl;
    a_ = obj.a_;
    return *this;
  }

  Demo& operator=(Demo&& obj) {
    std::cout << "move = constructor demo, obj.a_=" << obj.a_ << std::endl;
    a_ = std::move(obj.a_);
    return *this;
  }

  ~Demo() { std::cout << "destructor demo, a_=" << a_ << std::endl; }

  const std::string& GetDetail() const { return a_; }

 public:
  std::string a_;
};

/// 静态数据中心
class StaticDataCenter {
 public:
  StaticDataCenter() {
    // vector不reserve，看看效果
    values_.emplace_back(new Demo("static_刘德华"), true);
    values_.emplace_back(new Demo("static_张学友"), true);
  }

  /// GetValue返回的结果不带有资源owner权，不负责释放
  UniqueHandler<Demo> GetValue(uint32_t index) {
    if (index >= values_.size()) {
      return UniqueHandler<Demo>(nullptr);
    }
    return values_[index].Shadow();
  }

 private:
  std::vector<UniqueHandler<Demo>> values_;
};

/// 动态数据中心
class DynamicDataCenter {
 public:
  DynamicDataCenter() {
    values_.reserve(2);  // 不预留时，vector空间的动态增长会触发一次拷贝
    values_.emplace_back("dynamic_刘德华");
    values_.emplace_back("dynamic_张学友");
  }

  /// GetValue返回的结果带有资源owner权，负责释放
  UniqueHandler<Demo> GetValue(uint32_t index) {
    if (index >= values_.size()) {
      return UniqueHandler<Demo>(nullptr);
    }
    UniqueHandler<Demo> value(new Demo(values_[index]), true);
    return value;
  }

 private:
  std::vector<Demo> values_;  ///< 动态数据中心的数据是从磁盘读取，这里伪造为从内存获取
};

void PrintValue(const UniqueHandler<Demo>& value) {
  if (value) {
    std::cout << "value=" << value->GetDetail() << std::endl;
  }
}

int main() {
  StaticDataCenter static_center;
  std::cout << std::endl << std::endl;
  DynamicDataCenter dynamic_center;

  std::cout << std::endl << std::endl;
  std::cout << "----StaticDataCenter GetValue()----" << std::endl;
  {
    auto item = static_center.GetValue(0);
    PrintValue(item);
  }
  std::cout << "----StaticDataCenter GetValue() end----" << std::endl;
  std::cout << std::endl << std::endl;
  std::cout << "----DynamicDataCenter GetValue()----" << std::endl;
  {
    auto item = dynamic_center.GetValue(0);
    auto item1 = dynamic_center.GetValue(1);
    if (item1 == nullptr) {
      std::cout << "is null" << std::endl;
    }
    item1 = std::move(item);
    PrintValue(item1);
  }
  std::cout << "----DynamicDataCenter GetValue() end----" << std::endl;
  std::cout << std::endl << std::endl;

  return 0;
}

/*
constructor demo, a=static_刘德华
constructor demo, a=static_张学友


constructor demo, a=dynamic_刘德华
constructor demo, a=dynamic_张学友


----StaticDataCenter GetValue()----
value=static_刘德华
----StaticDataCenter GetValue() end----


----DynamicDataCenter GetValue()----
copy constructor demo, obj.a_=dynamic_刘德华
copy constructor demo, obj.a_=dynamic_张学友
destructor demo, a_=dynamic_张学友
value=dynamic_刘德华
destructor demo, a_=dynamic_刘德华
----DynamicDataCenter GetValue() end----


destructor demo, a_=dynamic_刘德华
destructor demo, a_=dynamic_张学友
destructor demo, a_=static_刘德华
destructor demo, a_=static_张学友
*/