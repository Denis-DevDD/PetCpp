#include "GameError.hpp"
#include "CardDatabase.hpp"
#include "Utils.hpp"

std::map<std::string, CardData> CardDatabase::card_data_;

Trait CardDatabase::stringToTrait(const std::string& trait)
{
  if (trait == "Brutal")
  {
    return Trait::BRUTAL;
  }
  else if (trait == "Challenger")
  {
    return Trait::CHALLENGER;
  }
  else if (trait == "First Strike")
  {
    return Trait::FIRST_STRIKE;
  }
  else if (trait == "Haste")
  {
    return Trait::HASTE;
  }
  else if (trait == "Lifesteal")
  {
    return Trait::LIFESTEAL;
  }
  else if (trait == "Poisoned")
  {
    return Trait::POISONED;
  }
  else if (trait == "Regenerate")
  {
    return Trait::REGENERATE;
  }
  else if (trait == "Temporary")
  {
    return Trait::TEMPORARY;
  }
  else if (trait == "Undying")
  {
    return Trait::UNDYING;
  }
  else
  {
    return Trait::VENOMOUS;
  }
}

std::string CardDatabase::traitToString(const Trait& trait)
{
  switch (trait)
  {
    case Trait::BRUTAL:
      return "Brutal";
    case Trait::CHALLENGER:
      return "Challenger";
    case Trait::FIRST_STRIKE:
      return "First Strike";
    case Trait::HASTE:
      return "Haste";
    case Trait::LIFESTEAL:
      return "Lifesteal";
    case Trait::POISONED:
      return "Poisoned";
    case Trait::REGENERATE:
      return "Regenerate";
    case Trait::TEMPORARY:
      return "Temporary";
    case Trait::UNDYING:
      return "Undying";
    case Trait::VENOMOUS:
      return "Venomous";
    default:
      return "Unknown";
  }
}

void CardDatabase::loadSpellCardInfo(const std::string& path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    throw InvalidFile();
  }

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream ss(line);
    std::string token;

    std::vector<std::string> parts;
    while (std::getline(ss, token, ';'))
    {
      parts.push_back(token);
    }

    CardData spell;
    spell.type_ = CardType::SPELL;
    if (parts[0] != "XX")
    {
      Utils::decimalStringToInt(parts[0], spell.mana_cost_);
    }
    else
    {
      spell.mana_cost_ = 0;
    }
    spell.id_ = parts[1];
    spell.name_ = parts[2];
    card_data_[spell.id_] = spell;
  }
}

void CardDatabase::loadCreatureCardInfo(const std::string& path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    throw InvalidFile();
  }

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream ss(line);
    std::string token;

    std::vector<std::string> parts;
    while (std::getline(ss, token, ';'))
    {
      parts.push_back(token);
    }

    CardData creature;
    creature.type_ = CardType::CREATURE;
    Utils::decimalStringToInt(parts[0], creature.mana_cost_);
    creature.id_ = parts[1];
    creature.name_ = parts[2];

    for (size_t i = 3; i < parts.size() - 2; i++)
    {
      if (parts[i] == "-")
      {
        break;
      }
      creature.traits_.push_back(stringToTrait(parts[i]));
    }

    std::sort(creature.traits_.begin(), creature.traits_.end());

    Utils::decimalStringToInt(parts[parts.size() - 2], creature.base_attack_);
    Utils::decimalStringToInt(parts[parts.size() - 1], creature.base_health_);

    card_data_[creature.id_] = creature;
  }
}

void CardDatabase::loadCardInfo(const std::string& spell_path, const std::string& creature_path)
{
  loadSpellCardInfo(spell_path);
  loadCreatureCardInfo(creature_path);
}

std::optional<CardData> CardDatabase::getInfo(const std::string& card_id)
{
  auto card = card_data_.find(card_id);

  if (card != card_data_.end())
  {
    return card->second;
  }
  return std::nullopt;
}

bool CardDatabase::isIdCorrect(const std::string& card_id)
{
  auto card = card_data_.find(card_id);

  if (card != card_data_.end())
  {
    return true;
  }
  return false;
}
