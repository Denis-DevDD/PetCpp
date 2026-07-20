
#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <string>
#include <map>
#include <vector>
#include "Command.hpp"
#include "Game.hpp"
class Game;
class CommandParser
{
private:
  std::map<std::string, Command*> commands_;
  std::vector<std::string> split_input_;
  std::vector<int> position_;
  bool exception_thrown_;

  void removeEndsWhitespaces(std::string& user_input);

  void removeBeginingsWhitespaces(std::string& user_input);

  void splitStringtoVector(const std::string& user_input);

  void extractPositionParameters(std::vector<std::string> split_input);

  void splitStringByComma(std::string string_to_split, std::vector<std::string>& all_split_strings);

public:

  CommandParser() = default;

  std::map<std::string, Command*> getAllCommands();

  void registerCommand(const std::string name_of_command, Command* command);


  int readCommand();

  bool executeCommand(std::string user_input, Game* game);

  ~CommandParser();

  CommandParser(const CommandParser&) = delete;

  std::vector<std::string> getUserInput();

  std::vector<int> getCrd();

  bool getIfThrow() {return exception_thrown_;}

  void setParsersValues(std::string& command, std::string& Id, std::string item, std::pair<int, int>& params);
};
#endif
