/**
 * @file vector_string_join.cc
 * @brief 验证vector<string> join，set也可以用这个方式join
 * 参考：https://en.cppreference.com/w/cpp/algorithm/copy
 */
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main() {
  {
    std::vector<std::string> items{"hello", "world", "123"};
    std::stringstream ss;
    std::copy(items.begin(), items.end(), std::ostream_iterator<std::string>(ss, "|"));
    std::cout << ss.str() << std::endl;
    // 输出：hello|world|123|
  }
  {
    std::vector<int> items{1, 2, 3, 4};
    std::stringstream ss;
    std::copy(items.begin(), items.end(), std::ostream_iterator<int>(ss, "|"));
    std::cout << ss.str() << std::endl;
    // 输出：1|2|3|4|
  }

  return 0;
}
