#include "Utils.hpp"

bool Utils::decimalStringToInt(const std::string& str, int& number)
{
  size_t position = 0;

  try
  {
    number = std::stoul(str, &position, 10);
  }
  catch (std::exception&)
  {
    return false;
  }

  return position == str.length();
}

bool Utils::decimalStringToFloat(const std::string& str, float& floating)
{
  size_t position = 0;

  try
  {
    floating = std::stof(str, &position);
  }
  catch (std::exception&)
  {
    return false;
  }

  return position == str.length();
}
