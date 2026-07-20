#include "Creature.hpp"

Creature::Creature(std::shared_ptr<Player> owner, const std::string& id, const std::string& name,
  int manacost, int health, int attack, std::vector<Trait> traits) :
  Card(owner, id, name, manacost), base_health_(health), current_health_(health), base_attack_(attack),
  current_attack_(attack), base_traits_(std::move(traits)), current_traits_(base_traits_), is_locked_(false) {}

std::string Creature::getTraitsToPrint() const
{
  std::vector<std::string> trait_names;

  for (Trait trait : current_traits_)
  {
    switch (trait)
    {
      case Trait::BRUTAL:
        trait_names.push_back("B");
        break;
      case Trait::CHALLENGER:
        trait_names.push_back("C");
        break;
      case Trait::FIRST_STRIKE:
        trait_names.push_back("F");
        break;
      case Trait::HASTE:
        trait_names.push_back("H");
        break;
      case Trait::LIFESTEAL:
        trait_names.push_back("L");
        break;
      case Trait::POISONED:
        trait_names.push_back("P");
        break;
      case Trait::REGENERATE:
        trait_names.push_back("R");
        break;
      case Trait::TEMPORARY:
        trait_names.push_back("T");
        break;
      case Trait::UNDYING:
        trait_names.push_back("U");
        break;
      case Trait::VENOMOUS:
        trait_names.push_back("V");
        break;
    }
  }

  std::sort(trait_names.begin(), trait_names.end());

  std::string result;

  if (trait_names.size() > 5)
  {
    for (int i = 0; i < 4; ++i)
    {
      result += trait_names[i];
    }
    result += "+ ";
  }
  else
  {
    for (const auto& letter : trait_names)
    {
      result += letter;
    }
    for (size_t i = 0; i < 6 - trait_names.size(); ++i)
    {
      result += ' ';
    }
  }
  return result;
}

std::string Creature::getCardRowString(int row) const
{
  switch (row)
  {
    case 0:
    {
      return " _____M0" + std::to_string(manacost_);
    }
    case 1:
    {
      return "| " + id_ + " |";
    }
    case 2:
    {
      return "| " + getTraitsToPrint() + "|";
    }
    case 3:
    {
      std::string attack_output = (current_attack_ < 10 ? "0" : "") + std::to_string(current_attack_);
      std::string health_output  = (current_health_ < 10 ? "0" : "") + std::to_string(current_health_);
      return "A" + attack_output + "___H" + health_output;
    }
    default:
      return "";
  }
}

bool Creature::hasTrait(Trait trait) const
{
  for (auto& creature_trait : current_traits_)
  {
    if (creature_trait == trait)
    {
      return true;
    }
  }
  return false;
}

void Creature::removeAlphabeticallyFirstTrait()
{
  if (!current_traits_.empty())
  {
    auto it = std::min_element(current_traits_.begin(), current_traits_.end());
    if (it != current_traits_.end())
    {
      current_traits_.erase(it);
    }
  }
}

void Creature::changeStats(int attack, int health)
{
  current_attack_ += attack;
  current_health_ += health;
}

std::vector<int> Creature::getStats() const
{
  return {current_attack_, current_health_};
}

void Creature::addTrait(Trait trait)
{
  current_traits_.push_back(trait);
}

void Creature::resetCard()
{
  current_attack_ = base_attack_;
  current_health_ = base_health_;
  current_traits_ = base_traits_;
}

bool Creature::isCreature() const
{
  return true;
}

void Creature::resetStats()
{
  current_health_ = base_health_;
  current_attack_ = base_attack_;
}

void Creature::removeTrait(Trait trait)
{
  auto it = std::remove(current_traits_.begin(), current_traits_.end(), trait);
  if (it != current_traits_.end())
  {
    current_traits_.erase(it, current_traits_.end());
  }
}

std::vector<Trait> Creature::getTraits() const
{
 return current_traits_;
}
