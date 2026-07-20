#include "Board.hpp"
#include "Messages.hpp"
#include "Creature.hpp"

std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> Board::field_zone_;
std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> Board::battle_zone_;

void Board::setBoard(const std::vector<std::shared_ptr<Player>>& players)
{
  for (const auto& player : players)
  {
    field_zone_[player] = std::vector<std::shared_ptr<Card>>(7);
    battle_zone_[player] = std::vector<std::shared_ptr<Card>>(7);
  }
}

void Board::addCardToFieldZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card, int slot_id)
{
  field_zone_.at(player).at(slot_id - 1) = card;
}

void Board::addCardToBattleZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card, int slot_id)
{
  battle_zone_.at(player).at(slot_id - 1) = card;
}

void Board::removeCardFromFieldZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card)
{
  for (auto& slot : field_zone_.at(player))
  {
    if (slot == card)
    {
      slot = nullptr;
      return;
    }
  }
}

void Board::removeCardFromBattleZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card)
{
  for (auto& slot : field_zone_.at(player))
  {
    if (slot == card)
    {
      slot = nullptr;
      return;
    }
  }
}

void Board::resolveTemporary(std::shared_ptr<Player> player)
{
  std::vector<std::shared_ptr<Card>>& battle_zone = battle_zone_.at(player);
  std::vector<std::shared_ptr<Card>>& field_zone = field_zone_.at(player);
  for (auto& slot : battle_zone)
  {
    if (slot)
    {
      if (slot)
      {
        player->addToGrave(slot);
        slot.reset();
      }
    }
  }
  for (auto& slot : field_zone)
  {
    if (slot)
    {
      if (slot)
      {
        player->addToGrave(slot);
        slot.reset();
      }
    }
  }
}

void Board::resolveOther(std::shared_ptr<Player> player)
{
  std::vector<std::shared_ptr<Card>>& battle_zone = battle_zone_.at(player);
  std::vector<std::shared_ptr<Card>>& field_zone = field_zone_.at(player);
  size_t slot_number = 0;
  for (auto& slot : battle_zone)
  {
    if (!field_zone[slot_number])
    {
      field_zone[slot_number] = slot;
    }
    else
    {
      player->addToGrave(slot);
    }
    slot.reset();
    slot_number++;
  }
}

void Board::removeTemporary(std::vector<std::shared_ptr<Player>> players)
{
  for (auto& player : players)
  {
    for (auto& slot : battle_zone_.at(player))
    {
      if (slot)
      {
        auto creature = std::dynamic_pointer_cast<Creature>(slot);
        if (creature->hasTrait(Trait::TEMPORARY))
        {
          std::cout << "[INFO] " << Messages::getInfo("I_TEMPORARY") << std::endl;
          player->addToGrave(slot);
          slot = nullptr;
        }
      }
    }
    for (auto& slot : field_zone_.at(player))
    {
      if (slot)
      {
        auto creature = std::dynamic_pointer_cast<Creature>(slot);
        if (creature->hasTrait(Trait::TEMPORARY))
        {
          std::cout << "[INFO] " << Messages::getInfo("I_TEMPORARY") << std::endl;
          player->addToGrave(slot);
          slot = nullptr;
        }
      }
    }
  }
}

void Board::cleanupDeadCreatures(std::vector<std::shared_ptr<Player>> players)
{
  for (auto& player : players)
  {
    for (auto& slot : battle_zone_.at(player))
    {
      if (slot)
      {
        auto creature = std::dynamic_pointer_cast<Creature>(slot);
        if (creature->getCurrentHealth() <= 0)
        {
          player->addToGrave(slot);
          slot = nullptr;
        }
      }
    }
    for (auto& slot : field_zone_.at(player))
    {
      if (slot)
      {
        auto creature = std::dynamic_pointer_cast<Creature>(slot);
        if (creature->getCurrentHealth() <= 0)
        {
          player->addToGrave(slot);
          slot = nullptr;
        }
      }
    }
  }
}

void Board::returnToFieldZone(std::vector<std::shared_ptr<Player>> players)
{
  for (auto& player : players)
  {
    for (auto& battle_slot : battle_zone_.at(player))
    {
      if (battle_slot)
      {
        for (auto& field_slot : field_zone_.at(player))
        {
          if (!field_slot)
          {
            field_slot = battle_slot;
            battle_slot = nullptr;
            break;
          }
        }
        if (battle_slot)
        {
          player->addToGrave(battle_slot);
          battle_slot = nullptr;
        }
      }
    }
  }
}

void Board::handleUndying(std::vector<std::shared_ptr<Player>> players)
{
  for (auto& player : players)
  {
    auto graveyard = player->getGraveyard();

    for (int index = graveyard.size() - 1; index >= 0; index--)
    {
      const auto& creature = std::dynamic_pointer_cast<Creature>(graveyard.at(index));
      if (creature->hasTrait(Trait::UNDYING))
      {
        for (auto& slot : field_zone_.at(player))
        {
          if (!slot)
          {
            std::cout << "[INFO] " << Messages::getInfo("I_UNDYING") << std::endl;
            slot = graveyard.at(index);
            creature->resetCard();
            creature->removeTrait(Trait::UNDYING);
            player->removeFromGraveyard(graveyard.at(index));
            break;
          }
        }
      }
    }
  }
}

void Board::moveToGraveyard(std::shared_ptr<Player> player, std::shared_ptr<Card> card)
{
  for (auto& slot : battle_zone_.at(player))
  {
    if (slot == card)
    {
      slot = nullptr;
    }
  }

  for (auto& slot : field_zone_.at(player))
  {
    if (slot == card)
    {
      slot = nullptr;
    }
  }
  player->addToGrave(card);
}

void Board::addSpellCreature(const std::shared_ptr<Player> own, const std::shared_ptr<Card> creature)
{
  for (auto& [player, card_vec] : field_zone_)
  {
    if (player == own)
    {
      for (auto& card : card_vec)
      {
        if (!card)
        {
          card = creature;
          break;
        }
      }
    }
  }
}

void Board::unlockCards(std::vector<std::shared_ptr<Player>> players)
{
  for (auto& player : players)
  {
    for (auto& card : field_zone_.at(player))
    {
      if (card)
      {
        std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(card);
        creature->unlockCard();
      }
    }
  }
}

bool Board::isFieldSlotEmpty(std::shared_ptr<Player> player, int slot_id) const
{
  if (field_zone_.at(player)[slot_id - 1] == nullptr)
  {
    return true;
  }
  return false;
}

bool Board::isBattleSlotEmpty(std::shared_ptr<Player> player, int slot_id) const
{
  if (battle_zone_.at(player)[slot_id - 1] == nullptr)
  {
    return true;
  }
  return false;
}

void Board::printZone(std::shared_ptr<Player> player, ZoneType zone_type) const
{
  const int slots = 7;
  const int card_height = 4;

  std::string zone_id = (zone_type == ZoneType::BATTLE) ? "B" : "F";

  std::vector<std::shared_ptr<Card>> cards;
  if (zone_type == ZoneType::BATTLE)
  {
    cards = battle_zone_.at(player);
  }
  else
  {
    cards = field_zone_.at(player);
  }
  std::vector<std::string> output(card_height, zone_id + "   ");

  for (int row = 0; row < card_height; ++row)
  {
    for (int slot = 0; slot < slots; ++slot)
    {
      std::shared_ptr<Card> card = cards[slot];

      if (card)
      {
        output[row] += card->getCardRowString(row);
      }
      else
      {
        output[row] += std::string("         ");
      }
      output[row] += "   ";
    }
    output[row] += zone_id;
  }

  for (const auto& line : output)
  {
    std::cout << line << std::endl;
  }
}

void Board::printBoard(std::vector<std::shared_ptr<Player>> players) const
{
  int defender_id = (players[0]->getRole() == Role::DEFENDER) ? 0 : 1;
  int attacer_id = (defender_id + 1) % 2;
  std::cout << "================================== DEFENDER: PLAYER "
    << defender_id + 1 << " ===================================\n";

  printZone(players[defender_id], ZoneType::FIELD);
  std::cout << Messages::getDescription("D_BORDER_A") << std::endl;
  printZone(players[defender_id], ZoneType::BATTLE);
  std::cout << Messages::getDescription("D_BORDER_B") << std::endl;
  printZone(players[attacer_id], ZoneType::BATTLE);
  std::cout << Messages::getDescription("D_BORDER_A") << std::endl;
  printZone(players[attacer_id], ZoneType::FIELD);

  std::cout << "================================== ATTACKER: PLAYER "
    << attacer_id + 1 << " ===================================\n";
}

const std::shared_ptr<Card>& Board::getFieldZoneSlot(std::shared_ptr<Player> player, int slot_id) const
{
  return field_zone_.at(player).at(slot_id - 1);
}

const std::shared_ptr<Card>& Board::getBattleZoneSlot(std::shared_ptr<Player> player, int slot_id) const
{
  return battle_zone_.at(player).at(slot_id - 1);
}

const std::vector<std::shared_ptr<Card>> Board::getPlayerFieldZone(const std::shared_ptr<Player>& player)
{
  return field_zone_.at(player);
}

const std::vector<std::shared_ptr<Card>> Board::getPlayerBattleZone(const std::shared_ptr<Player>& player)
{
  return battle_zone_.at(player);
}

const std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> Board::getFieldZone()
{
  return field_zone_;
}

const std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> Board::getBattleZone()
{
  return battle_zone_;
}
