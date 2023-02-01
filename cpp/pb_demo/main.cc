/**
 * @file main.cc
 * @brief protobuf 测试
 */
#include <fstream>
#include <sstream>

#include "demo.pb.h"
#include "google/protobuf/util/json_util.h"

template <typename T>
std::string ProtoBufToJson(const T& pb, bool add_whitespace = false, bool print_primitive_fields = true) {
  google::protobuf::util::JsonPrintOptions pb_json_option;
  pb_json_option.add_whitespace = add_whitespace;  // 是否添加空格
  pb_json_option.always_print_primitive_fields = print_primitive_fields;
  pb_json_option.preserve_proto_field_names = true;  // 保持原有字段名 不转驼峰

  std::string json;
  google::protobuf::util::MessageToJsonString(pb, &json, pb_json_option);
  return json;
}

template <typename T>
std::pair<bool, std::string> JsonToProtoBuf(const std::string& json, T* pb_ptr) {
  google::protobuf::util::JsonParseOptions parse_option;
  parse_option.ignore_unknown_fields = true;
  auto status = google::protobuf::util::JsonStringToMessage(json, pb_ptr, parse_option);
  if (!status.ok()) {
    return {false, status.ToString()};
  }

  return {true, ""};
}

void ReleaseDemoFile() {
  example::Document doc;
  doc.set_global_id("doc_id_12345");
  doc.set_title("送柴侍御");
  doc.set_content("流水通波接武冈，送君不觉有离伤。\r\n青山一道同云雨，明月何曾是两乡。");
  doc.set_score(10);
  std::cout << doc.Utf8DebugString() << std::endl;
  std::string pb_string = ProtoBufToJson(doc, false, false);
  std::ofstream of("./demo_proto_string");
  of << pb_string << std::endl;
  of.close();
}

std::string ReadFile(const std::string& file_path) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs) {
    return "";
  }
  std::stringstream content;
  content << ifs.rdbuf();
  return content.str();
}

void LoadDemoFile() {
  std::string content = ReadFile("./demo_proto_string");
  example::Document doc;
  JsonToProtoBuf(content, &doc);
  std::cout << doc.Utf8DebugString() << std::endl;
  std::cout << "score=" << doc.score() << std::endl;
}

int main() {
  std::cout << "-----------Release Demo File-----------" << std::endl;
  ReleaseDemoFile();
  std::cout << "-----------Load Demo File-----------" << std::endl;
  LoadDemoFile();
  return 0;
}
