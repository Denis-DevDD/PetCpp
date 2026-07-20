#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "Command.hpp"
#include "Game.hpp"

class CommandLine
{
  private:
    void removeTrailingWhitespaces(std::string &string);
    void removeLeadingWhitespace(std::string &string);
    void removeWhitespacesAtEnds(std::string &string);
    void stringToVector(const std::string &string, std::vector<std::string> &vector, char delimiter);

  public:
    CommandLine() = default;
    CommandLine(const CommandLine&) = delete;
    ~CommandLine() = default;
    Command readCommand();
};


#endif
