#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>
#include <iostream>
#include <string>

class CommandExceptions : public std::exception
{
private:
  std::string message_;
public:
  CommandExceptions(const std::string& message) : message_(message) {}
  virtual const char* what() const noexcept override
  {
    return message_.c_str();
  }
};

class GameExceptions : public std::exception
{
private:
  std::string message_;
public:
 GameExceptions(const std::string& message) : message_(message) {}
  virtual const char* what() const noexcept override
  {
    return message_.c_str();
  }
};


class WrongParamExceptions : public std::exception
{
private:
  std::string message_;
public:
 WrongParamExceptions(const std::string& message) : message_(message) {}
  virtual const char* what() const noexcept override
  {
    return message_.c_str();
  }
};

#endif
