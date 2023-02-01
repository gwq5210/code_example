/**
 * @file yaml_demo.cc
 * @brief 测试 yaml-cpp 接口的使用
 */

#include "yaml-cpp/yaml.h"

#include <iostream>
#include <string>

int main() {
  constexpr char kDemoInfo[] = R"(
config:
    table: 123
    timeout: hello
)";

  YAML::Node node = YAML::Load(kDemoInfo);
  if (!node.IsMap()) {
    std::cout << "not map" << std::endl;
  }

  std::string table_name = node["config"]["table"].as<std::string>("table_name");
  std::cout << "table_name=" << table_name << std::endl;

  uint32_t timeout = node["config"]["timeout"].as<uint32_t>(12);
  std::cout << "timeout=" << timeout << std::endl;

  return 0;
}
