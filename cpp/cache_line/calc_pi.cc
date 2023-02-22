#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

static constexpr int kThreadCount = 4;
static constexpr int kCalcN = 2000000000;

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

inline std::size_t GetCostUs(TimePoint start, TimePoint end) {
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

inline TimePoint NowTimePoint() { return std::chrono::steady_clock::now(); }

struct PiSum {
  double sum = 0.0;
  char padding[56];
};
PiSum pi_sums[kThreadCount];

void CalcPi(int index) {
  double mid = 0.0;
  double height = 0.0;
  double width = 1.0 / ((double)kCalcN);
  int step = kCalcN / kThreadCount;
  int start = step * index;
  int end = step * (index + 1);
  for (int i = start; i < end; ++i) {
    mid = (i + 0.5) * width;
    pi_sums[index].sum += 4.0 / (1 + mid * mid);
  }
  pi_sums[index].sum *= width;
}

int main() {
  TimePoint begin_time = NowTimePoint();
  double result = 0.0;
  std::vector<std::thread> tids;
  printf("the size of struct sum_struct = %zu\n", sizeof(PiSum));
  for (int i = 0; i < kThreadCount; i++) {
    pi_sums[i].sum = 0.0;
    tids.emplace_back(CalcPi, i);
  }
  for (int i = 0; i < kThreadCount; ++i) {
    tids[i].join();
    result += pi_sums[i].sum;
  }
  printf("the PI = %.32f\n", result);
  TimePoint end_time = NowTimePoint();
  printf("cost %zu(us), %zu(ms)\n", GetCostUs(begin_time, end_time), GetCostUs(begin_time, end_time) / 1000);
  return 0;
}