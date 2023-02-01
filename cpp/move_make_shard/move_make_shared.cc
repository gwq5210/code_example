/**
 * @file move_make_shared.cc
 * @brief 验证move进智能指针里的unordered_map对象，其value是否会发生拷贝：不会。
 */
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class MoveMakeShare {
 public:
  MoveMakeShare(uint32_t a) : a_(a) { std::cout << "constructor" << std::endl; }
  MoveMakeShare(const MoveMakeShare& obj) { std::cout << "copy constructor" << std::endl; }
  MoveMakeShare(MoveMakeShare&& obj) { std::cout << "move copy constructor" << std::endl; }

  ~MoveMakeShare() { std::cout << "destroctor" << std::endl; }

 private:
  int a_ = 0;
};

int main() {
  std::unordered_map<uint32_t, MoveMakeShare> infos;
  // 这里move或者不move都一样，都是：MoveMakeShare对象先构造出来，然后move进infos里
  infos.emplace(1, MoveMakeShare(10));
  infos.emplace(2, std::move(MoveMakeShare(20));
  std::cout << "---------start------------" << std::endl;
  // 这里会导致MoveMakeShare对象发生拷贝
  // auto shared_infos = std::make_shared<std::unordered_map<uint32_t, MoveMakeShare>>((infos));
  // 这里不会导致MoveMakeShare对象发生拷贝
  auto shared_infos = std::make_shared<std::unordered_map<uint32_t, MoveMakeShare>>(std::move(infos));
  std::cout << "---------end------------" << std::endl;

  return 0;
}

/*
constructor
move copy constructor
destroctor
constructor
move copy constructor
destroctor
---------start------------
---------end------------
destroctor
destroctor
 **/
