/**
 * @file json_str_diff.cc
 * @brief {@link json_str_diff.h}
 */

#include "json_str_diff.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

std::string VectorToString(const std::vector<std::string>& input) {
  std::stringstream ss;
  std::copy(input.begin(), input.end(), std::ostream_iterator<std::string>(ss, "|"));
  return ss.str();
}

bool JsonStrDiff::CheckDiff(const std::string& a, const std::string& b) {
  rapidjson::Document doc_a;
  rapidjson::ParseResult parse_ret_a = doc_a.Parse(a);
  if (!parse_ret_a) {
    std::stringstream ss;
    ss << "doc a parse err,code=" << rapidjson::GetParseError_En(parse_ret_a.Code())
       << ",offset=" << parse_ret_a.Offset();
    std::cout << ss.str() << std::endl;
    return true;
  }
  rapidjson::Document doc_b;
  rapidjson::ParseResult parse_ret_b = doc_b.Parse(b);
  if (!parse_ret_b) {
    std::stringstream ss;
    ss << "doc b parse err,code=" << rapidjson::GetParseError_En(parse_ret_b.Code())
       << ",offset=" << parse_ret_b.Offset();
    std::cout << ss.str() << std::endl;
    return true;
  }
  return CheckDocumentDiff(doc_a, doc_b);
}

std::string JsonStrDiff::SerializeDiffDetail() {
  std::stringstream ss;
  uint32_t index = 0;
  for (const auto& item : diff_detail_) {
    std::string order = std::to_string(index);
    ss << order << ". " << item.first << ":" << std::endl;
    ss << order << ".1:" << item.second[0] << std::endl;
    ss << order << ".2:" << item.second[1] << std::endl << std::endl;
    ++index;
  }
  return ss.str();
}

std::string JsonStrDiff::SerializeCheckDetail() {
  std::stringstream ss;
  for (const auto& item : check_detail_) {
    ss << item << std::endl;
  }
  return ss.str();
}

bool JsonStrDiff::CheckDocumentDiff(const rapidjson::Document& a, const rapidjson::Document& b) {
  CheckValueDiff("", a, b);
  return !diff_detail_.empty();
}

void JsonStrDiff::CheckValueDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  rapidjson::Type a_type = a.GetType();
  rapidjson::Type b_type = b.GetType();
  if (a_type != b_type) {
    diff_detail_.emplace_back(parent_key + "|type",
                              std::vector<std::string>{std::to_string(a_type), std::to_string(b_type)});
    // 值的类型不对齐，后续的值不做一致性判断
    return;
  }

  switch (a_type) {
    case rapidjson::kObjectType:
      CheckObjectDiff(parent_key, a, b);
      break;
    case rapidjson::kArrayType:
      CheckArrayDiff(parent_key, a, b);
      break;
    case rapidjson::kStringType:
      CheckStringDiff(parent_key, a, b);
      break;
    case rapidjson::kNumberType:
      CheckNumberDiff(parent_key, a, b);
      break;
    case rapidjson::kTrueType:
    case rapidjson::kFalseType:
      CheckBoolDiff(parent_key, a, b);
      break;
    case rapidjson::kNullType:
      std::cout << "null type" << std::endl;
      break;
    default:
      std::cout << "unknow type" << std::endl;
  }
}

void JsonStrDiff::CheckObjectDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  std::set<std::string> a_keys;
  std::set<std::string> b_keys;
  for (auto it = a.MemberBegin(); it != a.MemberEnd(); ++it) {
    a_keys.insert(it->name.GetString());
  }
  for (auto it = b.MemberBegin(); it != b.MemberEnd(); ++it) {
    b_keys.insert(it->name.GetString());
  }

  // 先判断key集合是否一致
  if (a_keys != b_keys) {
    std::vector<std::string> a_b;
    std::vector<std::string> b_a;
    std::set_difference(a_keys.begin(), a_keys.end(), b_keys.begin(), b_keys.end(), std::back_inserter(a_b));
    std::set_difference(b_keys.begin(), b_keys.end(), a_keys.begin(), a_keys.end(), std::back_inserter(b_a));
    std::string a_b_string = VectorToString(a_b);
    std::string b_a_string = VectorToString(b_a);
    diff_detail_.emplace_back(parent_key + "|keys", std::vector<std::string>{a_b_string, b_a_string});
  }

  // 再判断相同key对应的value是否一致
  std::vector<std::string> a_and_b;
  std::set_intersection(a_keys.begin(), a_keys.end(), b_keys.begin(), b_keys.end(), std::back_inserter(a_and_b));
  for (const auto& key : a_and_b) {
    const rapidjson::Value& a_value = a[key];
    const rapidjson::Value& b_value = b[key];
    CheckValueDiff(parent_key + "|" + key, a_value, b_value);
  }
}

void JsonStrDiff::CheckArrayDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  rapidjson::SizeType a_size = a.Size();
  rapidjson::SizeType b_size = b.Size();
  if (a_size != b_size) {
    diff_detail_.emplace_back(parent_key + "|array_size",
                              std::vector<std::string>{std::to_string(a_size), std::to_string(b_size)});
    // 数组长度不对齐，后续的数组元素不做一致性对比
    return;
  }

  for (uint32_t i = 0; i != a_size; ++i) {
    const auto& a_item = a[i];
    const auto& b_item = b[i];
    CheckValueDiff(parent_key + "|array[" + std::to_string(i) + "]", a_item, b_item);
  }
}

void JsonStrDiff::CheckStringDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  std::string a_item = a.GetString();
  std::string b_item = b.GetString();
  check_detail_.push_back(parent_key + ":" + a_item + "|" + b_item);
  if (a_item != b_item) {
    diff_detail_.emplace_back(parent_key + "|string", std::vector<std::string>{a_item, b_item});
  }
}

void JsonStrDiff::CheckNumberDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  double a_num = a.GetFloat();
  double b_num = b.GetFloat();
  check_detail_.push_back(parent_key + ":" + std::to_string(a_num) + "|" + std::to_string(b_num));
  if (std::fabs(a_num - b_num) > std::numeric_limits<float>::epsilon()) {
    // 数字只用float精度
    diff_detail_.emplace_back(parent_key + "|number",
                              std::vector<std::string>{std::to_string(a_num), std::to_string(b_num)});
  }
}

void JsonStrDiff::CheckBoolDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b) {
  check_detail_.push_back(parent_key + ":" + std::to_string(a.GetBool()) + "|" + std::to_string(b.GetBool()));
  if (a.GetBool() != b.GetBool()) {
    diff_detail_.emplace_back(parent_key + "|bool",
                              std::vector<std::string>{std::to_string(a.GetBool()), std::to_string(b.GetBool())});
  }
}
