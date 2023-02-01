/**
 * @file string_replace_if.cpp
 * @brief 测试 std::replace_if、std::regex_replace，例子：将给定的一串字符里的某些（多个）字符替换为空格
 * 使用 std::regex_replace 代码最简洁
 */

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>

std::string ReplaceCopy(const std::string& input, const char* old_values, char new_value) {
  std::string result(input.length(), '\0');
  std::replace_copy_if(
      input.begin(), input.end(), result.begin(),
      [old_values](char x) {
        const char* p = old_values;
        while (*p != '\0') {
          if (x == *p) {
            return true;
          }
          ++p;
        }
        return false;
      },
      new_value);
  return result;
}

void ReplaceInPlace(std::string& input, const char* old_values, char new_value) {
  std::replace_if(
      input.begin(), input.end(),
      [old_values](char x) {
        const char* p = old_values;
        while (*p != '\0') {
          if (x == *p) {
            return true;
          }
          ++p;
        }
        return false;
      },
      new_value);
}

void TestRepalceCopy() {
  std::string input = "刘德华\r\n|朱丽倩|\r\n";
  std::string result = ReplaceCopy(input, "|\r\n", ' ');
  std::cout << "old:" << input << std::endl;
  std::cout << "new:" << result << std::endl;
}

void TestRepalceInplace() {
  std::string input = "刘德华\r\n|朱丽倩|\r\n";
  std::cout << "old:" << input << std::endl;
  ReplaceInPlace(input, "|\r\n", ' ');
  std::cout << "new:" << input << std::endl;
}

void TestRepalceCopyRegex() {
  std::string input = "\r\r\n\n刘德华\r\n|朱|丽|倩|\r\n";
  std::string new_str = std::regex_replace(input, std::regex{"\\||\\r|\\n"}, " ");
  std::cout << "old:" << input << std::endl;
  std::cout << "new:" << new_str << std::endl;
}

int main() {
  std::cout << "-----TestRepalceCopy()-----" << std::endl;
  TestRepalceCopy();
  std::cout << "-----TestRepalceInplace()-----" << std::endl;
  TestRepalceInplace();

  std::cout << "-----TestRepalceInplace use regex-----" << std::endl;
  TestRepalceCopyRegex();
}