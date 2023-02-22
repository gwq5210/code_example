#include <chrono>
#include <cstdio>
#include <functional>
#include <semaphore>
#include <thread>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

inline std::size_t GetCostUs(TimePoint start, TimePoint end) {
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

inline TimePoint NowTimePoint() { return std::chrono::steady_clock::now(); }

static constexpr int kCacheLineSize = 64;

struct FalseSharingData {
  int a = 0;
  int b = 0;
};

struct SharingData {
  int a = 0;
  alignas(kCacheLineSize) int b = 0;
};

void Add(int& v, int count) {
  TimePoint begin_time = NowTimePoint();
  for (int i = 0; i < count; ++i) {
    ++v;
  }
  TimePoint end_time = NowTimePoint();
  printf("cost %zu(us), %zu(ms)\n", GetCostUs(begin_time, end_time), GetCostUs(begin_time, end_time) / 1000);
}

int main(int argc, char* argv[]) {
  SharingData s_data;
  FalseSharingData fs_data;
  printf("s_data %p, a %p, b %p\n", &s_data, &s_data.a, &s_data.b);
  printf("fs_data %p, a %p, b %p\n", &fs_data, &fs_data.a, &fs_data.b);

  int count = 1000000000;
  std::thread s_thread_add_a(Add, std::ref(s_data.a), count);
  std::thread s_thread_add_b(Add, std::ref(s_data.b), count);

  s_thread_add_a.join();
  s_thread_add_b.join();

  std::thread fs_thread_add_a(Add, std::ref(fs_data.a), count);
  std::thread fs_thread_add_b(Add, std::ref(fs_data.b), count);

  fs_thread_add_a.join();
  fs_thread_add_b.join();

  printf("s_data a %d, b %d\n", s_data.a, s_data.b);
  printf("fs_data a %d, b %d\n", fs_data.a, fs_data.b);
  return 0;
}