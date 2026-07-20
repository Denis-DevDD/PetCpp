#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Utils
{
public:
  Utils() = delete;
  Utils(const Utils&) = delete;
  ~Utils() = delete;

  static bool decimalStringToInt(const std::string& str, int& int_number);

  static bool decimalStringToFloat(const std::string& string, float& float_number);
};

#endif
