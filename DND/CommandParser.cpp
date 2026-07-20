#include "CommandParser.hpp"
#include "Functions.hpp"
#include "Exceptions.hpp"

void CommandParser::registerCommand(const std::string name_of_command, Command* command)
{
  commands_.insert({name_of_command, command});
}



bool CommandParser::executeCommand(std::string user_input, Game* game)
{
  if (commands_.count(user_input) == 0)
  {
    return false;
  }

  commands_.at(user_input)->execute(game);
  return true;
}

int CommandParser::readCommand()
{
  try {
  split_input_.clear();
  position_.clear();
  std::cout << "> ";
  std::string user_input;
  try
  {
    std::getline(std::cin, user_input);
  }
  catch(const std::bad_alloc& e)
  {
    exception_thrown_ = true;
  }

  if (std::cin.eof())
  {
    return 1;
  }
  Functions::allToUpper(user_input);
  removeEndsWhitespaces(user_input);
  removeBeginingsWhitespaces(user_input);
  splitStringtoVector(user_input);
  extractPositionParameters(split_input_);
  exception_thrown_ = false;
  }
  catch (const WrongParamExceptions& e)
  {
    exception_thrown_ = true;
  }
  return 0;
}

void CommandParser::removeEndsWhitespaces(std::string& user_input)
{
  size_t end = user_input.find_last_not_of(' ');
  user_input = end == std::string::npos ? "" : user_input.substr(0, end + 1);
}

void CommandParser::removeBeginingsWhitespaces(std::string& user_input)
{
  auto start = user_input.find_first_not_of(' ');
  user_input = start == std::string::npos ? "" : user_input.substr(start);
}

void CommandParser::splitStringtoVector(const std::string& user_input)
{
  size_t start = 0;
  size_t end;
  std::string word_to_store;

  while((end = user_input.find_first_of(' ', start)) != std::string::npos)
  {
    word_to_store = user_input.substr(start, end - start);
    start = end + 1;
    if (!word_to_store.empty())
    {
        split_input_.push_back(word_to_store);
    }
  }

  split_input_.push_back(user_input.substr(start));
  if (split_input_[0].empty())
  {
    split_input_[0] = "KJAWKNFJK";
  }
}

void CommandParser::extractPositionParameters(std::vector<std::string> split_input)
{
  std::vector<std::string> all_split_strings;
  int amount_of_split_strings = 0;
  for (auto& i : split_input)
  {
    splitStringByComma(i, all_split_strings);
    if (size(all_split_strings) != 0)
    {


      amount_of_split_strings = amount_of_split_strings + size(all_split_strings);
      if(amount_of_split_strings != 2)
      {
        throw CommandExceptions("E_INVALID_PARAM_COUNT");
      }
  }
  if (size(all_split_strings) != 0)
  {
    position_.push_back(Functions::stringToInteger(all_split_strings[0]));
    position_.push_back(Functions::stringToInteger(all_split_strings[1]));
  }
  }
}

void CommandParser::splitStringByComma(std::string string_to_split,std::vector<std::string>& all_split_strings)
{
  size_t start = 0;
  size_t end;
  std::string paramter_to_store;

  if (string_to_split.find_last_of(' ') != std::string::npos)
  {
    start = string_to_split.find_last_of(' ')  + 1;
  }

  while((end = string_to_split.find_first_of(',', start)) != std::string::npos)
  {
    paramter_to_store = string_to_split.substr(start, end - start);
    start = end + 1;
    if (!paramter_to_store.empty())
    {
        all_split_strings.push_back(paramter_to_store);
    }
    else
    {
      throw CommandExceptions("E_INVALID_PARAM_COUNT");
    }
    end = size(string_to_split);
    paramter_to_store = string_to_split.substr(start, end - start);
    removeBeginingsWhitespaces(paramter_to_store);
    removeEndsWhitespaces(paramter_to_store);
    if (!paramter_to_store.empty())
    {
        all_split_strings.push_back(paramter_to_store);
    }
    else
    {
      throw CommandExceptions("E_INVALID_PARAM_COUNT");
    }
  }

}

CommandParser::~CommandParser()
{
  for (const auto& pair : commands_)
  {
    delete pair.second;
  }
}

std::map<std::string, Command*> CommandParser::getAllCommands()
{
  return commands_;
}

std::vector<std::string> CommandParser::getUserInput()
{
  return split_input_;
}

std::vector<int> CommandParser::getCrd()
{
  return position_;
}

void CommandParser::setParsersValues(std::string& command, std::string& id, std::string item, std::pair<int, int>& params)
{
  split_input_.clear();
  position_.clear();
  Functions::allToUpper(command);
  Functions::allToUpper(id);
  split_input_.push_back(command);
  split_input_.push_back(id);
  split_input_.push_back(item);
  position_.push_back(params.first);
  position_.push_back(params.second);
}
