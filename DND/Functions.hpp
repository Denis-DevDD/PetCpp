#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <iostream>
#include "Enemy.hpp"
#include "Player.hpp"
class Functions
{
public:
  Functions() = delete;
  Functions(const Functions&) = delete;
  ~Functions() = delete;

  static int stringToInteger(std::string string_to_convert);

  static void allToUpper(std::string& string_to_convert);


  static void allToLower(std::string& string_to_convert);

  static int checkInitialAmount(std::string& input_amount);

  static int doClassInput(int& w_players, int& b_players, int& r_players);

  static bool compareEnemies(const Enemy* a, const Enemy* b);

  static bool compareItems(const std::pair<Item*, int>& a, const std::pair<Item*, int>& b);

  static bool canHit(int y_f, int x_f, int y_s, int x_s);

  static bool compareFields(Field* a, Field* b);

  static void Damagecheck(int& dmg, const int health);

  static bool comparePlayers(const std::pair<Player*, int>& a, const std::pair<Player*, int>& b);

  static bool getPositionsAdjacent(int my_x, int my_y, int other_x, int other_y);
};
#endif
