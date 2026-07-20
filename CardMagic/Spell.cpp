#include "Spell.hpp"

Spell::Spell(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost, SpellType type) :
  Card(owner, id, name, manacost), type_{type} {}

std::string Spell::getCardRowString(int row) const
{
  switch (row)
  {
    case 0:
    {
      std::string output = (manacost_ == 0) ? "XX" : ("0" + std::to_string(manacost_));
      return " _____M" + output;
    }
    case 1:
    {
      return "| " + id_ + " |";
    }
    case 2:
    {
      return "|       |";
    }
    case 3:
    {
      return " _______ ";
    }
    default:
      return "";
  }
}

std::vector<std::string> Spell::spellTarget(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target;
  const std::string& slot = parameters[1];
  size_t i = 0;

  while (i < slot.size() && !std::isdigit(slot[i]))
  {
    target.push_back(std::string(1, slot[i]));
    ++i;
  }

  if (i < slot.size())
  {
    target.push_back(slot.substr(i));
  }
  return target;
}

std::shared_ptr<Creature> Spell::checkAndReturnTarget(const std::vector<std::string> target) const
{
  auto field = Board::getFieldZone();
  auto battle = Board::getBattleZone();

  if ((target.size() != 2 && target.size() != 3) ||
    (target.size() == 2 && target[0] != "F" && target[0] != "B") ||
    (target.size() == 3 && (target[0] != "O" || (target[1] != "F" && target[1] != "B"))))
  {
    throw InvalidSlotSpell();
  }

  std::shared_ptr<Player> own = owner_.lock();

  bool is_opponent = (target.size() == 3);
  bool is_field = (target[is_opponent ? 1 : 0] == "F");

  int slot_id;
  Utils::decimalStringToInt(target[is_opponent ? 2 : 1], slot_id);

  if (slot_id < 0 || slot_id > 7)
  {
    throw InvalidSlotSpell();
  }

  auto& zone_map = is_field ? field : battle;

  std::vector<std::shared_ptr<Card>>* zone = nullptr;

  for (auto& [player, cards] : zone_map)
  {
    if ((is_opponent && player != own) || (!is_opponent && player == own))
    {
      zone = &cards;
      break;
    }
  }

  if (!zone || slot_id > static_cast<int>(zone->size()) || !(*zone)[slot_id - 1])
  {
    throw TargetEmptyException();
  }

  auto creature = std::dynamic_pointer_cast<Creature>((*zone)[slot_id - 1]);
  if (!creature)
  {
    throw TargetEmptyException();
  }
  return creature;
}

std::shared_ptr<Card> Spell::checkAndReturnGrave(const std::string& id) const
{
  std::shared_ptr<Player> own = owner_.lock();
  std::vector<std::shared_ptr<Card>> grave = own -> getGraveyard();
  std::shared_ptr<Creature> target;
  for (int index = own->getGraveyardSize() - 1; index >= 0; index--)
  {
    if (grave.at(index)->getId() == id)
    {
      return grave.at(index);
    }
  }
  throw NotInGraveyardException();
}

bool Spell::isCreature() const
{
  return false;
}

SpellType Spell::getType() const
{
  return type_;
}

void BattleCry::executeSpell(const std::vector<std::string>& parameters)
{
  (void)parameters;
  std::shared_ptr<Player> owner = owner_.lock();
  std::vector<std::shared_ptr<Card>> field = Board::getPlayerFieldZone(owner);
  std::vector<std::shared_ptr<Card>> battle = Board::getPlayerBattleZone(owner);
  std::vector<std::shared_ptr<Card>> combined;
  combined.insert(combined.end(), field.begin(), field.end());
  combined.insert(combined.end(), battle.begin(), battle.end());

  for (auto& card : combined)
  {
    if (card)
    {
      auto creature = std::dynamic_pointer_cast<Creature>(card);
      if (creature)
      {
        std::vector<Trait> provided = creature->getTraits();
        if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
        {
          creature->addTrait(Trait::HASTE);
        }

        if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
        {
          creature->addTrait(Trait::TEMPORARY);
        }
        creature->changeStats(3, 0);
      }
    }
  }
}

void Meteor::executeSpell(const std::vector<std::string>& parameters)
{
  (void)parameters;
  std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> field = Board::getFieldZone();
  std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> battle = Board::getBattleZone();
  for (const auto& [player, card_vec] : field)
  {
    for (const auto& card : card_vec)
    {
      auto creature = std::dynamic_pointer_cast<Creature>(card);
      if (creature)
      {
        creature->changeStats(0, -3);
      }
    }
  }
  for (const auto& [player, card_vec] : battle)
  {
    for (const auto& card : card_vec)
    {
      auto creature = std::dynamic_pointer_cast<Creature>(card);
      if (creature)
      {
        creature->changeStats(0, -3);
      }
    }
  }
}

void Fireball::executeSpell(const std::vector<std::string>& parameters)
{
  (void)parameters;
  std::shared_ptr<Player> owner = owner_.lock();
  std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> field = Board::getFieldZone();
  std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> battle = Board::getBattleZone();
  for (const auto& [player, card_vec] : field)
  {
    if (player != owner)
    {
      for (const auto& card : card_vec)
      {
        if (card != nullptr)
        {
          auto creature = std::dynamic_pointer_cast<Creature>(card);
          creature->changeStats(0, -2);
        }
      }
    }
  }
  for (const auto& [player, card_vec] : battle)
  {
    if (player != owner)
    {
      for (const auto& card : card_vec)
      {
        if (card != nullptr)
        {
          auto creature = std::dynamic_pointer_cast<Creature>(card);
          creature->changeStats(0, -2);
        }
      }
    }
  }
}

void HeroicMem::executeSpell(const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2)
  {
    throw InvalidParamCountSpellException();
  }
  std::shared_ptr<Card> target;
  try
  {
    target = checkAndReturnGrave(parameters[1]);
  }
  catch(const std::exception& e)
  {
    throw;
  }

  bool is_field_full = true;
  std::shared_ptr<Creature> mem_creature = std::dynamic_pointer_cast<Creature>(target);
  std::shared_ptr<Player> owner = owner_.lock();

  for (auto& slot : Board::getPlayerFieldZone(owner))
  {
    if (!slot)
    {
      is_field_full = false;
    }
  }

  if (!is_field_full)
  {
    std::vector<Trait> provided = mem_creature->getTraits();
    mem_creature->resetCard();
    if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
    {
      mem_creature->addTrait(Trait::HASTE);
    }
    if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
    {
      mem_creature->addTrait(Trait::TEMPORARY);
    }
    Board::addSpellCreature(owner, mem_creature);
    owner->removeFromGraveyard(target);
  }

  int mana = (mem_creature->getManaCost());
  if (mana % 2 != 0)
  {
    mana = (mana / 2) + 1;
  }
  else
  {
    mana = mana / 2;
  }
  this -> setMana(mana);
}

void Revive::executeSpell(const std::vector<std::string>& parameters)
{
  if (parameters.size() != 2)
  {
    throw InvalidParamCountSpellException();
  }
  std::shared_ptr<Card> target;
  try
  {
    target = checkAndReturnGrave(parameters[1]);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(target);
  creature->resetCard();
  std::shared_ptr<Player> owner = owner_.lock();
  owner->addCardToHand(creature);
  owner->removeFromGraveyard(target);
}

void Clone::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::shared_ptr<Creature> cloned_creature = std::make_shared<Creature> (*creature);
  std::vector<Trait> provided = cloned_creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
  {
    cloned_creature->addTrait(Trait::HASTE);
  }
  if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
  {
    cloned_creature->addTrait(Trait::TEMPORARY);
  }
  std::shared_ptr<Player> owner = owner_.lock();
  Board::addSpellCreature(owner, cloned_creature);

  int mana = (creature->getManaCost());
  if (mana % 2 != 0)
  {
    mana = (mana / 2) + 1;
  }
  else
  {
    mana = mana / 2;
  }
  this -> setMana(mana);
}

void DeathCurse::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
  {
    creature->addTrait(Trait::TEMPORARY);
  }
  int mana = (creature->getManaCost()) + 1;
  this->setMana(mana);
}

void Shock::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }

  creature->changeStats(0, -1);
}


void Mobilize::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
  {
    creature->addTrait(Trait::HASTE);
  }
  creature->changeStats(1, 0);
}

void RapidRush::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::FIRST_STRIKE) == provided.end())
  {
    creature->addTrait(Trait::FIRST_STRIKE);
  }
  if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
  {
    creature->addTrait(Trait::TEMPORARY);
  }
  creature->changeStats(2, 0);
}

void Shield::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  creature->changeStats(0, 2);
}

void Amputate::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  creature -> removeAlphabeticallyFirstTrait();
}

void Final::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::BRUTAL) == provided.end())
  {
    creature->addTrait(Trait::BRUTAL);
  }
  if (std::find(provided.begin(), provided.end(), Trait::TEMPORARY) == provided.end())
  {
    creature->addTrait(Trait::TEMPORARY);
  }
  if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
  {
    creature->addTrait(Trait::HASTE);
  }
  creature->changeStats(3, 0);
}

void Loyalty::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::HASTE) == provided.end())
  {
    creature->addTrait(Trait::HASTE);
  }
  creature->changeStats(0, 1);
}

void Zombify::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::VENOMOUS) == provided.end())
  {
    creature->addTrait(Trait::VENOMOUS);
  }
  if (std::find(provided.begin(), provided.end(), Trait::UNDYING) == provided.end())
  {
    creature->addTrait(Trait::UNDYING);
  }
}

void Bloodlust::executeSpell(const std::vector<std::string>& parameters)
{
  std::vector<std::string> target = spellTarget(parameters);
  std::shared_ptr<Creature> creature;
  try
  {
    creature = checkAndReturnTarget(target);
  }
  catch(const std::exception& e)
  {
    throw;
  }
  std::vector<Trait> provided = creature->getTraits();
  if (std::find(provided.begin(), provided.end(), Trait::BRUTAL) == provided.end())
  {
    creature->addTrait(Trait::BRUTAL);
  }
  if (std::find(provided.begin(), provided.end(), Trait::LIFESTEAL) == provided.end())
  {
    creature->addTrait(Trait::LIFESTEAL);
  }
  int health_reduction = (creature -> getStats())[1] / 2;
  creature->changeStats(0, -health_reduction);
}
