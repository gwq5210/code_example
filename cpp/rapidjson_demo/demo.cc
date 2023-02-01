/**
 * @file demo.cc
 * @brief rapidjson demo
 */
#include <iomanip>
#include <iostream>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

/*
int main() {
  rapidjson::StringBuffer buf;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
  writer.StartObject();

  writer.Key("stage");
  writer.Int(static_cast<int>(11));
  writer.Key("stage_name");
  writer.String("valuexxxx");
  writer.EndObject();

  std::cout << buf.GetString() << std::endl;

  return 0;
}
*/

int main() {
  std::vector<std::string> items{"hello", "world"};
  rapidjson::Document obj(rapidjson::kArrayType);
  rapidjson::Document::AllocatorType& alloc = obj.GetAllocator();
  for (const auto& item : items) {
    obj.PushBack({item, alloc}, alloc);
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  writer.SetMaxDecimalPlaces(3);
  obj.Accept(writer);
  std::cout << buffer.GetString() << std::endl;
  return 0;
}
