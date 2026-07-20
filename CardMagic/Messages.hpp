#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

class Messages
{
  private:
    static std::map<std::string, std::string> error_messages_;
    static std::map<std::string, std::string> info_messages_;
    static std::map<std::string, std::string> description_messages_;

  public:
    static int loadMessages(const char* path);
    static std::string getError(const std::string& key)
    {
      return error_messages_.at(key);
    }
    static std::string getInfo(const std::string& key)
    {
      return info_messages_.at(key);
    }
    static std::string getDescription(const std::string& key)
    {
      return description_messages_.at(key);
    }
};

#endif
