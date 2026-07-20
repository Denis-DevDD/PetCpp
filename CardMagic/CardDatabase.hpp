#ifndef CARDDATABASE_HPP
#define CARDDATABASE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Creature.hpp"

enum class CardType
{
  SPELL,
  CREATURE
};

struct CardData
{
  std::string id_;
  std::string name_;
  int mana_cost_;
  CardType type_;
  std::vector<Trait> traits_ {};
  int base_attack_ = 0;
  int base_health_ = 0;
};

class CardDatabase
{
  private:
    static std::map<std::string, CardData> card_data_;

  public:
    static Trait stringToTrait(const std::string& str);
    static std::string traitToString(const Trait& trait);
    static void loadSpellCardInfo(const std::string& path);
    static void loadCreatureCardInfo(const std::string& path);
    static void loadCardInfo(const std::string& spell_path, const std::string& creature_path);
    static std::optional<CardData> getInfo(const std::string& card_id);
    static bool isIdCorrect(const std::string& card_id);
};

#endif
