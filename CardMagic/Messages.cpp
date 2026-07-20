#include "Messages.hpp"
#include "GameError.hpp"

std::map<std::string, std::string> Messages::error_messages_;
std::map<std::string, std::string> Messages::info_messages_;
std::map<std::string, std::string> Messages::description_messages_;

int Messages::loadMessages(const char* path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    return 0;
  }

  std::string line;
  std::getline(file, line);

  if (line != "MESSAGE")
  {
    return 0;
  }

  while (std::getline(file, line))
  {
    std::stringstream ss(line);
    std::string key;
    std::string message;

    std::getline(ss, key, ':');
    std::getline(ss, message);

    if (key.at(0) == 'E')
    {
      error_messages_.insert({key, message});
    }
    else if (key.at(0) == 'I')
    {
      info_messages_.insert({key, message});
    }
    else
    {
      description_messages_.insert({key, message});
    }
  }
  return 1;
}
