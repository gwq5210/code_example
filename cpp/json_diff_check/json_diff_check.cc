/**
 * @file json_diff_check.cc
 * @brief 对任意层级嵌套的数据做diff检查
 */

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "json_str_diff.h"

std::string ReadTotalFile(const std::string& file_path) {
  // rdbuf + movestring，内存double
  std::ifstream in(file_path);
  std::ostringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "use: exe a.file b.file" << std::endl;
    return -1;
  }

  std::string content_a = ReadTotalFile(argv[1]);
  std::string content_b = ReadTotalFile(argv[2]);
  if (content_a.empty() || content_b.empty()) {
    std::cerr << "file empty" << std::endl;
    return -1;
  }

  JsonStrDiff diff;
  bool has_diff = diff.CheckDiff(content_a, content_b);
  std::cout << "check detail:" << std::endl << diff.SerializeCheckDetail() << std::endl;
  std::cout << "diff detail:" << std::endl << diff.SerializeDiffDetail() << std::endl;
  std::cout << "has diff:" << has_diff << std::endl;
  return 0;
}
