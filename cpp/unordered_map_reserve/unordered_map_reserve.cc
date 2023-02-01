/**
 * @file unordered_map_reserve.cc
 * @brief 验证unordered_map的reserve的价值， 使用reserve可以减少耗时
 * 接口介绍：https://www.cplusplus.com/reference/unordered_map/unordered_map/reserve/
 */
#include <sys/time.h>

#include <iostream>
#include <string>
#include <unordered_map>

/**
 * @brief 借助gettimeofday实现的微秒精度计时器
 */
class CostTime {
 public:
  CostTime();

  /**
   * @brief 重新开始计时
   */
  void Restart();
  /**
   * @brief 计时到现在过了多少毫秒
   * @return double类型的毫秒，小数点后三位有效数字
   */
  double CostMs();

 private:
  struct timeval start_tv_;  ///< 计时开始时刻
};

CostTime::CostTime() { ::gettimeofday(&start_tv_, nullptr); }

void CostTime::Restart() { ::gettimeofday(&start_tv_, nullptr); }

double CostTime::CostMs() {
  struct timeval end_tv;
  ::gettimeofday(&end_tv, nullptr);
  return (end_tv.tv_sec - start_tv_.tv_sec) * 1000 + static_cast<double>(end_tv.tv_usec - start_tv_.tv_usec) / 1000;
}

void NoUseReserve(uint32_t count) {
  std::unordered_map<std::string, std::string> datas;
  for (uint32_t i = 0; i != count; ++i) {
    datas.emplace(std::to_string(i), "1");
  }
}

void UseReserve(uint32_t count) {
  std::unordered_map<std::string, std::string> datas;
  datas.reserve(count);
  for (uint32_t i = 0; i != count; ++i) {
    datas.emplace(std::to_string(i), "1");
  }
}

int main() {
  {
    CostTime cost_time;
    NoUseReserve(1000);
    std::cout << "NoUseReserve cost ms=" << cost_time.CostMs() << std::endl;
  }
  {
    CostTime cost_time;
    UseReserve(1000);
    std::cout << "UseReserve cost ms=" << cost_time.CostMs() << std::endl;
  }
}

/**
NoUseReserve cost ms=0.768
UseReserve cost ms=0.665
*/