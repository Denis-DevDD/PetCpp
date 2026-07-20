#include "CommandLine.hpp"

void CommandLine::removeTrailingWhitespaces(std::string& string)
{
  size_t end = string.find_last_not_of(' ');
  string = end == std::string::npos ? "" : string.substr(0, end + 1);
}

void CommandLine::removeLeadingWhitespace(std::string& string)
{
  auto start = string.find_first_not_of(' ');
  string = start == std::string::npos ? "" : string.substr(start);
}

void CommandLine::removeWhitespacesAtEnds(std::string& string)
{
  removeLeadingWhitespace(string);
  removeTrailingWhitespaces(string);
}

void CommandLine::stringToVector(const std::string& string, std::vector<std::string>& vector, char delimiter)
{
  size_t position = 0;
  size_t position2;
  std::string split_word;
  while ((position2 = string.find(delimiter, position)) != std::string::npos)
  {
    split_word = string.substr(position, position2 - position);
    position = position2 + 1;
    removeWhitespacesAtEnds(split_word);
    if (!split_word.empty())
    {
      vector.push_back(split_word);
    }
  }
  vector.push_back(string.substr(position));
}

Command CommandLine::readCommand()
{
  std::string input;
  std::getline(std::cin, input);
  removeWhitespacesAtEnds(input);
  std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::toupper(c); });
  if (input.empty())
  {
    throw UnknownCommandException();
  }
  std::vector<std::string> vectorised_string;
  stringToVector(input, vectorised_string, ' ');
  Command command(vectorised_string);
  switch (command.getType())
  {
    case CommandType::QUIT:
      if (!command.getParameters().empty())
      {
        throw InvalidParametersCountException();
      }
      break;

    case CommandType::INFO:
      if (command.getParameters().size() != 1)
      {
        throw InvalidParametersCountException();
      }
      break;

    case CommandType::BOARD:
    case CommandType::HELP:
    case CommandType::GRAVEYARD:
    case CommandType::HAND:
    case CommandType::REDRAW:
    case CommandType::STATUS:
    case CommandType::DONE:
      if (!command.getParameters().empty())
      {
        throw InvalidParametersCountException();
      }
      break;

    case CommandType::BATTLE:
    case CommandType::CREATURE:
      if (command.getParameters().size() != 2)
      {
        throw InvalidParametersCountException();
      }
      break;

    case CommandType::SPELL:
      if (command.getParameters().empty())
      {
        throw MissingCardException();
      }
      break;
  }
  return command;
}
