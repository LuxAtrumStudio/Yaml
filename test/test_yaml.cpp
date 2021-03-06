#include "yaml.hpp"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"

class YamlTest : public testing::Test {
 protected:
  virtual void SetUp() {
    y2_["a"] = 2017;
    y2_["b"] = 3.1415;
    y2_["c"] = std::string("string");
    y2_["d"] = "const char*";
    y2_["e"] = true;
    y2_["list"][0] = 0;
    y2_["list"][1] = 1;
    y2_["list"][2] = 2;
    y2_["list"][3]["sub-dic"]["a"] = 1;
    y2_["list"][3]["sub-dic"]["b"] = 1604;
    y2_["list"][3]["sub-dic"]["c"] = "This is a long string Still going.";
    y2_["list"][4]["sub-list"][0] = "sub-list-0";
    y2_["list"][4]["sub-list"][1] = 4061;
    y2_["f"]["int"] = 1998;
    y2_["f"]["double"] = 19.98;
    y2_["f"]["string"] = "1998";
    y2_["f"]["bool"] = false;
  }
  virtual void TearDown() {}
  yaml::Yaml y1_, y2_;
};

TEST_F(YamlTest, DefaultConstructor) {
  EXPECT_EQ(y1_.is_valid(), false);
  EXPECT_EQ(y1_.size(), 0);
}

TEST_F(YamlTest, Empty) {
  EXPECT_EQ(y1_.empty(), true);
  EXPECT_EQ(y2_.empty(), false);
  EXPECT_EQ(y2_["a"].empty(), false);
  EXPECT_EQ(y2_["c"].empty(), false);
  EXPECT_EQ(y2_["list"].empty(), false);
}

TEST_F(YamlTest, Size) {
  EXPECT_EQ(y1_.size(), 0);
  EXPECT_EQ(y2_.size(), 7);
  EXPECT_EQ(y2_["a"].size(), 1);
  EXPECT_EQ(y2_["c"].size(), 6);
  EXPECT_EQ(y2_["list"].size(), 5);
}

TEST_F(YamlTest, MaxSize) {
  std::map<std::string, yaml::Yaml> tmp_map;
  std::vector<yaml::Yaml> tmp_vec;
  std::string tmp_str;
  EXPECT_EQ(y1_.max_size(), 0);
  EXPECT_EQ(y2_.max_size(), tmp_map.max_size());
  EXPECT_EQ(y2_["list"].max_size(), tmp_vec.max_size());
  EXPECT_EQ(y2_["a"].max_size(), 1);
  EXPECT_EQ(y2_["c"].max_size(), tmp_str.max_size());
}

TEST_F(YamlTest, Clear) {
  y2_["a"].clear();
  EXPECT_EQ(y2_["a"].get_int(), int());
  y2_["b"].clear();
  EXPECT_EQ(y2_["b"].get_float(), double());
  y2_["c"].clear();
  EXPECT_EQ(y2_["c"].get_string(), std::string());
  y2_["e"].clear();
  EXPECT_EQ(y2_["e"].get_bool(), bool());
  y2_["list"].clear();
  EXPECT_EQ(y2_["list"].size(), 0);
  y2_.clear();
  EXPECT_EQ(y2_.size(), 0);
}

TEST_F(YamlTest, PushBack) {
  y1_.push_back(2017);
  y1_.push_back(3.1415);
  y1_.push_back(std::string("String"));
  y1_.push_back("const char*");
  y1_.push_back(true);
  EXPECT_EQ(y1_[0].get_int(), 2017);
  EXPECT_EQ(y1_[1].get_float(), 3.1415);
  EXPECT_EQ(y1_[2].get_string(), std::string("String"));
  EXPECT_EQ(y1_[3].get_string(), "const char*");
  EXPECT_EQ(y1_[4].get_bool(), true);
  y1_.push_back("INT", 2017);
  y1_.push_back("FLOAT", 3.1415);
  y1_.push_back("STRING", std::string("String"));
  y1_.push_back("CHAR", "const char*");
  y1_.push_back("BOOL", true);
  EXPECT_EQ(y1_["INT"].get_int(), 2017);
  EXPECT_EQ(y1_["FLOAT"].get_float(), 3.1415);
  EXPECT_EQ(y1_["STRING"].get_string(), std::string("String"));
  EXPECT_EQ(y1_["CHAR"].get_string(), "const char*");
  EXPECT_EQ(y1_["BOOL"].get_bool(), true);
}

TEST_F(YamlTest, Insert) {
  y2_["list"].insert(2, "Inserted");
  EXPECT_EQ(y2_["list"].size(), 6);
  EXPECT_EQ(y2_["list"][2].get_string(), "Inserted");
  y2_.insert("Insert_key", "Inserted");
  EXPECT_EQ(y2_["Insert_key"].get_string(), "Inserted");
}

TEST_F(YamlTest, Access) {
  EXPECT_EQ(y2_.access("a").get_int(), 2017);
  EXPECT_EQ(y2_["list"].access(10).get_type(), yaml::NONE);
  EXPECT_EQ(y2_["list"].size(), 11);
}

TEST_F(YamlTest, At) {
  EXPECT_EQ(y2_.at("list.2").get_int(), 2);
  EXPECT_EQ(y2_.at("list.4.sub-list.1").get_int(), 4061);
}

TEST_F(YamlTest, Conversion) {
  EXPECT_EQ(y2_["a"].get_int(), 2017);
  EXPECT_EQ(y2_["b"].get_float(), 3.1415);
  EXPECT_EQ(y2_["c"].get_string(), "string");
  EXPECT_EQ(y2_["d"].get_string(), "const char*");
  EXPECT_EQ(y2_["e"].get_bool(), true);
}

TEST_F(YamlTest, Find) {
  EXPECT_EQ(y2_.find("list"), y2_["list"]);
  EXPECT_EQ(y2_.find("f.int").get_int(), 1998);
  EXPECT_EQ(y2_.find("f").size(), 4);
  EXPECT_EQ(y2_("list.3.sub-dic.b").get_int(), 1604);
  EXPECT_EQ(y2_.find("list.2").get_int(), 2);
}

TEST_F(YamlTest, String) {
  std::string y2_str = y2_.String();
  std::string match =
      "a: 2017\n"
      "b: 3.1415\n"
      "c: string\n"
      "d: const char*\n"
      "e: true\n"
      "f: \n"
      "  bool: false\n"
      "  double: 19.98\n"
      "  int: 1998\n"
      "  string: 1998\n"
      "list: \n"
      "  - 0\n"
      "  - 1\n"
      "  - 2\n"
      "  - sub-dic: \n"
      "    a: 1\n"
      "    b: 1604\n"
      "    c: This is a long string Still going.\n"
      "  - sub-list: \n"
      "    - sub-list-0\n"
      "    - 4061";
  EXPECT_EQ(y2_str, match);
  EXPECT_EQ(y1_.String(), "(null)");
  std::stringstream ss;
  ss << y2_;
  y2_str = ss.str();
  EXPECT_EQ(y2_str, match);
}

TEST_F(YamlTest, Type) {
  EXPECT_EQ(y2_.get_type(), yaml::HASH);
  EXPECT_EQ(y2_["a"].get_type(), yaml::INT);
  EXPECT_EQ(y2_["b"].get_type(), yaml::FLOAT);
  EXPECT_EQ(y2_["c"].get_type(), yaml::STRING);
  EXPECT_EQ(y2_["d"].get_type(), yaml::STRING);
  EXPECT_EQ(y2_["e"].get_type(), yaml::BOOL);
  EXPECT_EQ(y2_["list"].get_type(), yaml::LIST);
  EXPECT_EQ(y2_["list"][0].get_type(), yaml::INT);
}

TEST_F(YamlTest, OperatorEQ) {
  EXPECT_EQ(y2_["a"] == y2_["b"], false);
  EXPECT_EQ(y2_ == y2_, true);
  EXPECT_EQ(y2_["list"] == y2_["list"][4]["sub-list"], false);
  EXPECT_EQ(y2_["e"] == y2_["f"]["bool"], false);
  EXPECT_EQ(y2_["b"] == y2_["f"]["double"], false);
  EXPECT_EQ(y2_["a"] == y2_["f"]["int"], false);
  EXPECT_EQ(y2_["c"] == y2_["c"], true);
}

TEST_F(YamlTest, Load) {
  std::string match_a =
      "a: 2017\n"
      "b: 3.1415\n"
      "c: string\n"
      "d: const char*\n"
      "e: true\n"
      "f:\n"
      "  bool: false\n"
      "  double: 19.98\n"
      "  int: 1998\n"
      "  string: 1998\n"
      "list:\n"
      "  - 0\n"
      "  - 1\n"
      "  - 2\n"
      "  - sub-dic:\n"
      "    a: 1\n"
      "    b: 1604\n"
      "    c: This is a long string Still going.\n"
      "  - sub-list:\n"
      "    - sub-list-0\n"
      "    - 4061\n";
  y1_ = yaml::LoadFile("test/sample.yml");
  EXPECT_EQ(y1_.String(), y2_.String());
  y1_.clear();
  y1_ = yaml::LoadString(match_a);
  EXPECT_EQ(y1_.String(), y2_.String());
}
