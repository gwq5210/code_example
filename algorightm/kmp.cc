#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

std::vector<int> GetNext(const std::string& target) {
  int i = 0;
  int j = -1;
  std::vector<int> next(target.size() + 1);
  next[0] = -1;
  while (i < target.size()) {
    if (j == -1 || target[i] == target[j]) {
      ++i;
      ++j;
      next[i] = j;
    } else {
      j = next[j];
    }
  }
  return next;
}

int KMP(const std::string& str, const std::string& target) {
  int i = 0;
  int j = 0;
  std::vector<int> next = GetNext(target);
  while (i < (int)str.size() && j < (int)target.size()) {
    if (j == -1 || str[i] == target[j]) {
      ++i;
      ++j;
    } else {
      j = next[j];
    }
  }
  if (j == target.size()) {
    return i - j;
  } else {
    return -1;
  }
}

int main(int argc, char* argv[]) {
  KMP("sadbutsad", "sad");
  return 0;
}