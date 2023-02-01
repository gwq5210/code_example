/**
 * @file bswap_example.cc
 * @brief 验证 bswap 函数(小端转大端)实现字节顺序转换
 */
#include <byteswap.h>

#include <iostream>
#include <sstream>

int main() {
  uint32_t size = 0;
  char* p = reinterpret_cast<char*>(&size);
  p[0] = 0x12;
  p[1] = 0x34;
  p[2] = 0x56;
  p[3] = 0x78;
  {
    std::stringstream ss;
    ss << std::hex << size;
    std::cout << "before 0x" << ss.str() << std::endl;
  }

  // 系统函数实现的转换
  uint32_t reserve_1 = bswap_32(size);
  {
    std::stringstream ss;
    ss << std::hex << reserve_1;
    std::cout << "after 0x" << ss.str() << std::endl;
  }

  // 自己实现的转换
  int32_t doc_size = p[3] & 0xFF;
  doc_size |= (p[2] << 8 & 0xFF00);
  doc_size |= (p[1] << 16 & 0xFF0000);
  doc_size |= (p[0] << 24 & 0xFF000000);
  {
    std::stringstream ss;
    ss << std::hex << doc_size;
    std::cout << "after_1 0x" << ss.str() << std::endl;
  }
  return 0;
}

/*
before 0x78563412
after 0x12345678
after_1 0x12345678
*/
