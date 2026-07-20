#include "Functions.hpp"
#include <string>
#include "Exceptions.hpp"
#include "Enemy.hpp"
#include <iostream>
int Functions::stringToInteger(std::string string_to_convert)
{
  int returning_integer = 0;
  int sizeof_string = size(string_to_convert);
  int amountof_integers = 0;
  for (auto& i : string_to_convert)
  {
    if(isdigit(i))
    {
      amountof_integers++;
    } else
    {
      throw WrongParamExceptions("E_INVALID_PARAM");
    }
  }
  if (amountof_integers == sizeof_string)
  {
    returning_integer = stoi(string_to_convert);
  }
  return returning_integer;
}

void Functions::allToUpper(std::string& string_to_convert)
{
  for (auto& c : string_to_convert)
  {
    c = toupper(c);
  }
}

void Functions::allToLower(std::string& string_to_convert)
{
  for (auto& c : string_to_convert)
  {
    c = tolower(c);
  }
}


int Functions::checkInitialAmount(std::string& input_amount)
{
  int amount;

  size_t end = input_amount.find_last_not_of(' ');
  input_amount = end == std::string::npos ? "" : input_amount.substr(0, end + 1);

  auto start = input_amount.find_first_not_of(' ');
  input_amount = start == std::string::npos ? "" : input_amount.substr(start);
  if (input_amount.empty())
  {
    throw WrongParamExceptions("");
  }
  allToUpper(input_amount);

  if (input_amount == "QUIT")
  {
    return -1;
  }
  amount = stringToInteger(input_amount);
  return amount;
}

int Functions::doClassInput(int& w_players, int& b_players, int& r_players)
{
  std::string input;
  bool valid_input = false;
  while (!valid_input)
  {
    std::getline(std::cin, input);

    if(std::cin.eof())
    {
      return -1;
    }
    allToUpper(input);
    if(input == "QUIT")
    {
      return -1;
    }
    if (input.size() > 1)
    {
      std::cout<<"Please enter a letter representing your desired player type (W, B, or R)."<<std::endl;
      std::cout<<"> ";
      valid_input = false;
    } else if(input == "W")
    {
      if (w_players >= 1)
      {
      std::cout<<"This player type is no longer available. Please choose a different player type."<<std::endl;
      std::cout<<"> ";
      valid_input = false;
      } else
      {
        w_players++;
        return 1;
      }
    }  else if(input == "B")
    {
      if (b_players >= 1)
      {
      std::cout<<"This player type is no longer available. Please choose a different player type."<<std::endl;
      std::cout<<"> ";
      valid_input = false;
      } else
      {
        b_players++;
        return 2;
      }
    }  else if(input == "R")
    {
      if (r_players >= 1)
      {
      std::cout<<"This player type is no longer available. Please choose a different player type."<<std::endl;
      std::cout<<"> ";
      valid_input = false;
      } else
      {
        r_players++;
        return 3;
      }
    } else
    {
      std::cout<<"Please enter a letter representing your desired player type (W, B, or R)."<<std::endl;
      std::cout<<"> ";
      valid_input = false;
    }

  }
return 0;
}

bool Functions::compareEnemies(const Enemy* a, const Enemy* b)
{
  if (a->getId() == b->getId())
  {
    return a->getNumber() < b->getNumber();
  }
  return a->getId() < b->getId();
}

bool Functions::compareItems(const std::pair<Item*, int>& a, const std::pair<Item*, int>& b)
{
  return a.first->getFullName() < b.first->getFullName();
}



bool Functions::canHit(int y_f, int x_f, int y_s, int x_s)
{
  if (x_f - x_s >= -1 && x_f - x_s <= 1)
  {
    if (y_f - y_s >= -1 && y_f - y_s <= 1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }

}

bool Functions::compareFields(Field* a, Field* b)
{
  int y_1 = a->getY();
  int y_2 = b->getY();
  if (y_1 != y_2)
  {
    return y_1 < y_2;
  }
  else
  {
    int x_1 = a->getX();
    int x_2 = b->getX();
    return x_1 <x_2;
  }
}

void Functions::Damagecheck(int& dmg, const int health)
{

  if (dmg > health)
  {
    dmg = health;
  }
    if (dmg < 0)
  {
    dmg = 0;
  }
}

bool Functions::comparePlayers(const std::pair<Player*, int>& a, const std::pair<Player*, int>& b)
{
  if (a.second != b.second)
  {
    return a.second < b.second;
  }
    return a.first->getHP() < b.first->getHP();
}

bool Functions::getPositionsAdjacent(int my_x, int my_y, int other_x, int other_y)
{
  int dx = std::abs(my_x - other_x);
  int dy = std::abs(my_y - other_y);
  return (dx <= 1 && dy <= 1) && !(dx == 0 && dy == 0);
}
