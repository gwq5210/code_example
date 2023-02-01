/**
 * @file list_dir.cc
 * @brief 获取目录下的所有文件
 */
#include <dirent.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> ListDir(const std::string& file_path) {
  std::vector<std::string> dir_names;
  DIR* dir;
  dirent* dirent;
  if ((dir = opendir(file_path.c_str())) == nullptr) {
    return dir_names;
  }

  while ((dirent = readdir(dir)) != nullptr) {
    std::cout << dirent->d_name << std::endl;
    std::cout << dirent->d_type << std::endl;
    if (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, "..") || dirent->d_type != DT_DIR) {
      continue;
    }
    dir_names.emplace_back(std::move(file_path + "/" + dirent->d_name + "/"));
  }
  closedir(dir);
  std::sort(dir_names.begin(), dir_names.end());
  std::cout << dir_names.size() << std::endl;
  return dir_names;
}

int main() {
  std::vector<std::string> contents = ListDir("./");
  for (const auto& item : contents) {
    std::cout << item << std::endl;
  }
  return 0;
}
