/**
 * @file json_str_diff.h
 * @brief 对任意层级嵌套的json数据做diff检查
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/// @brief 对任意层级的json做diff判断
class JsonStrDiff {
 public:
  /// @brief 是否有diff
  /// @param a json字符串a
  /// @param b json字符串b
  /// @return true 表示有diff
  bool CheckDiff(const std::string& a, const std::string& b);

  /// @brief  序列化diff详情返回
  /// @return diff详情
  std::string SerializeDiffDetail();

  /// @brief 序列化检查过程详情返回
  /// @return 检查过程详情
  std::string SerializeCheckDetail();

 private:
  bool CheckDocumentDiff(const rapidjson::Document& a, const rapidjson::Document& b);
  void CheckValueDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);
  void CheckObjectDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);
  void CheckArrayDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);
  void CheckStringDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);
  void CheckNumberDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);
  void CheckBoolDiff(const std::string& parent_key, const rapidjson::Value& a, const rapidjson::Value& b);

 private:
  std::vector<std::pair<std::string, std::vector<std::string>>> diff_detail_;  ///< diff详情
  std::vector<std::string> check_detail_;  ///< 检查的叶子节点值，按检查顺序排列. 叶子节点指：字符串、数字、bool类型值
};
