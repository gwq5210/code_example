/**
 * @file make_shared_float_array.cc
 * @brief
 * 1、通过make_shared构造数组，需要带上释放参数；
 * 2、通过make_unique则不需要；
 */
#include <iostream>
#include <memory>

constexpr uint32_t kArraySize = 10;

struct Demo {
  Demo() { std::cout << "constructor" << std::endl; }
  ~Demo() { std::cout << "destroy" << std::endl; }
  int index = 0;
};

void ShowArray(float* array, uint32_t size) {
  for (uint32_t i = 0; i != size; ++i) {
    std::cout << array[i] << std::endl;
  }
}

void ShowArray(Demo* array, uint32_t size) {
  for (uint32_t i = 0; i != size; ++i) {
    std::cout << array[i].index << std::endl;
  }
}

int UniquePtrFloatArray() {
  std::unique_ptr<float[]> floats = std::make_unique<float[]>(kArraySize);
  for (uint32_t i = 0; i != kArraySize; ++i) {
    floats.get()[i] = 0.2;
  }

  ShowArray(floats.get(), kArraySize);
  return 0;
}

int SharedPtrFloatArray() {
  std::shared_ptr<float> floats(new float[kArraySize], std::default_delete<float[]>());
  for (uint32_t i = 0; i != kArraySize; ++i) {
    std::cout << "index:" << i << std::endl;
    floats.get()[i] = 0.2;
  }

  ShowArray(floats.get(), kArraySize);
  return 0;
}

int SharedPtrDemoArray() {
  std::shared_ptr<Demo> demos(new Demo[kArraySize], std::default_delete<Demo[]>());
  // 下面的写法会导致core
  // std::shared_ptr<Demo> demos(new Demo[kArraySize]);
  for (uint32_t i = 0; i != kArraySize; ++i) {
    demos.get()[i].index = i;
  }

  ShowArray(demos.get(), kArraySize);
  return 0;
}

int UniquePtrDemoArray() {
  std::unique_ptr<Demo[]> demos(new Demo[kArraySize]);
  for (uint32_t i = 0; i != kArraySize; ++i) {
    demos.get()[i].index = i;
  }

  ShowArray(demos.get(), kArraySize);
  return 0;
}

int main() {
  UniquePtrFloatArray();
  SharedPtrFloatArray();
  UniquePtrDemoArray();
  SharedPtrDemoArray();
  return 0;
}
