/**
 * @file read_total_file.cc
 * @brief 测试读取文件性能和内存占用，
 * 1、性能最佳的方式是采用C API，次佳的方式是mmap+string， 第三是采用ifstream+rdbuf
 * 2、内存占用最佳是C API，次佳是ifstream+iterator
 * ifstream+iterator读取耗时极高，不建议采用
 */

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>

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

// std::string file_path = "/data/src/depend/data/kd_news_avail_20210321_shard_0.tar";

std::string file_path = "/data/src/depend/data/10w_qb_hot_query";

void ReadTotalFileV1() {
  // ifstream+iterator，性能最差，内存占用1.5倍，shrink_to_fit之后内存降低到1倍
  CostTime cost_time;
  std::ifstream in(file_path);
  std::string input((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  input.shrink_to_fit();
  std::cout << "size=" << input.size() << std::endl;
  std::cout << "v1 cost=" << cost_time.CostMs() << std::endl;

  // std::this_thread::sleep_for(std::chrono::seconds(60));
}

void ReadTotalFileV2() {
  // rdbuf + 拷贝string，内存double
  CostTime cost_time;
  std::ifstream in(file_path);
  std::ostringstream buffer;
  buffer << in.rdbuf();
  std::string input(buffer.str());
  std::cout << "size=" << input.size() << std::endl;
  std::cout << "v2 cost=" << cost_time.CostMs() << std::endl;

  // std::this_thread::sleep_for(std::chrono::seconds(30));
}

void ReadTotalFileV3() {
  // rdbuf + movestring，内存double
  CostTime cost_time;
  std::ifstream in(file_path);
  std::ostringstream buffer;
  buffer << in.rdbuf();
  std::string input(std::move(buffer.str()));
  std::cout << "size=" << input.size() << std::endl;
  std::cout << "v3 cost=" << cost_time.CostMs() << std::endl;

  // std::this_thread::sleep_for(std::chrono::seconds(30));
}

void ReadTotalFileV4() {
  // C API读取，最快，内存1倍
  // https://man7.org/linux/man-pages/man2/read.2.html
  // 一次最多读取0x7ffff000 (2,147,479,552) bytes, 所以需要while读取

  CostTime cost_time;
  int fd = open(file_path.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cout << "open file fail,path=" << file_path << std::endl;
    return;
  }
  struct stat st;
  fstat(fd, &st);
  char* buffer = new char[st.st_size];
  int64_t read_size = 0;
  while (read_size != st.st_size) {
    auto ret = read(fd, buffer + read_size, st.st_size - read_size);
    if (ret == -1) {
      std::cout << "read error,err=" << strerror(errno) << std::endl;
      return;
    }
    read_size += ret;
  }
  if (read_size != st.st_size) {
    std::cout << "read error, readsize=" << read_size << ",filesize=" << st.st_size << std::endl;
    return;
  }
  std::cout << "size=" << read_size << std::endl;
  std::cout << "v4 cost=" << cost_time.CostMs() << std::endl;

  // std::this_thread::sleep_for(std::chrono::seconds(30));
}

void ReadTotalFileV5() {
  // mmap + string，内存一倍
  CostTime cost_time;
  int fd = open(file_path.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cout << "open file fail,path=" << file_path << std::endl;
    return;
  }
  struct stat st;
  fstat(fd, &st);
  char* mem = static_cast<char*>(mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (mem == MAP_FAILED) {
    std::cout << "mmap file fail,path=" << file_path << ",reason=" << strerror(errno) << std::endl;
    return;
  }
  std::string input(mem, st.st_size);
  std::cout << "size=" << input.size() << std::endl;
  std::cout << "v5 cost=" << cost_time.CostMs() << std::endl;

  // std::this_thread::sleep_for(std::chrono::seconds(30));
}

int main() {
  ReadTotalFileV1();
  ReadTotalFileV2();
  ReadTotalFileV3();
  ReadTotalFileV4();
  ReadTotalFileV5();

  ReadTotalFileV1();
  ReadTotalFileV2();
  ReadTotalFileV3();
  ReadTotalFileV4();
  ReadTotalFileV5();

  return 0;
}

/*
总结：
1、原始C API读取速度最快，然后是mmap，第三是rdbuf+move
2、内存占用:原始C API和mmap最低，只要一倍；次之是ifstream+iterator
输出：
size=10053172
v1 cost=409.026
size=10053172
v2 cost=15.613
size=10053172
v3 cost=15.253
size=10053172
v4 cost=4.288
size=10053172
v5 cost=8.078

size=10053172
v1 cost=396.301
size=10053172
v2 cost=14.803
size=10053172
v3 cost=14.509
size=10053172
v4 cost=5.074
size=10053172
v5 cost=7.872

**/
