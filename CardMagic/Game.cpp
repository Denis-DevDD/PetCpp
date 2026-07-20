#include "Game.hpp"

Game::Game() : board_(), current_round_(0), max_rounds_(0), initial_mana_pool_size_(0), current_phase_(Phase::SETUP),
  board_printing_(true) {}

int Game::loadConfigFile(const char* path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    return 0;
  }

  config_file_path_ = path;
  std::string line;
  std::getline(file, line);
  if (line != "GAME")
  {
    return 0;
  }
  CardDatabase::loadCardInfo("data/spells.txt", "data/creatures.txt");

  int values[4];
  for (int& val : values)
  {
    std::getline(file, line);
    Utils::decimalStringToInt(line, val);
  }
  std::getline(file, line);

  max_rounds_ = values[1];
  initial_mana_pool_size_ = values[3];

  for (int i = 0; i < 2; i++)
  {
    std::getline(file, line);
    try
    {
      std::shared_ptr<Player> player = std::make_shared<Player>(values[0], values[3]);

      std::stringstream ss(line);
      std::string id;

      while (std::getline(ss, id, ';'))
      {
        auto card = CardCreator::createCardById(id, player);
        player->addCardToDeck(card);
      }

      players_.push_back(player);
    }
    catch (const std::exception&)
    {
      return 1;
    }
    board_.setBoard(players_);
  }
  return 2;
}

bool Game::loadMessages(const char* path)
{
  return Messages::loadMessages(path);
}

void Game::start()
{
  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
  std::cout << Messages::getDescription("D_WELCOME") << std::endl;
  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;

  active_player_ = players_[0];
  players_[0]->drawCards(6);
  players_[1]->drawCards(6);


  players_[0]->setRole(Role::ATTACKER);
  players_[1]->setRole(Role::DEFENDER);
}

void Game::swapRole()
{
  players_[0]->setRole((players_[0]->getRole() == Role::DEFENDER) ? Role::ATTACKER : Role::DEFENDER);
  players_[1]->setRole((players_[1]->getRole() == Role::DEFENDER) ? Role::ATTACKER : Role::DEFENDER);
}

bool Game::setRound()
{
  ++current_round_;
  if (current_round_ > max_rounds_)
  {
    if (players_[0]->getHealth() == players_[1]->getHealth())
    {
      players_[0]->makeWinner();
      players_[1]->makeWinner();
    }
    else if (players_[0]->getHealth() > players_[1]->getHealth())
    {
      players_[0]->makeWinner();
    }
    else
    {
      players_[1]->makeWinner();
    }
    endGame(GameEndReason::MAX_ROUNDS_EXCEEDED);
    return false;
  }

  std::cout << std::endl;
  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
  std::cout << "                                         ROUND " << current_round_ << std::endl;
  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;

  if (current_round_ % 2 == 0)
  {
    swapRole();
  }
  else
  {
    for (const auto& player : players_)
    {
      player->getManaPool().increaseManaPool(1);
      player->getManaPool().refill();
    }
  }

  if (board_printing_)
  {
    board_.printBoard(players_);
  }

  std::shared_ptr<Player> attacker = players_[(players_[0]->getRole() == Role::ATTACKER) ? 0 : 1];
  std::shared_ptr<Player> defender = players_[(players_[0]->getRole() == Role::ATTACKER) ? 1 : 0];
  active_player_ = attacker;

  board_.unlockCards(players_);

  if (!attacker->drawCards(1))
  {
    defender->makeWinner();
    endGame(GameEndReason::DRAW_CARD_FAILED);
    return false;
  }
  else if (!defender->drawCards(1))
  {
    attacker->makeWinner();
    endGame(GameEndReason::DRAW_CARD_FAILED);
    return false;
  }
  return true;
}

void Game::helpCommand()
{
  std::cout <<
  "=== Commands ============================================================================\n"
  "- help\n"
  "    Prints this help text.\n"
  "\n"
  "- quit\n"
  "    Terminates the game.\n"
  "\n"
  "- battle <FIELD_SLOT> <BATTLE_SLOT>\n"
  "    Moves a creature from a Field Zone slot into a Battle Zone slot.\n"
  "    <FIELD_SLOT>: Current slot of the creature (F1, ..., F7)\n"
  "    <BATTLE_SLOT>: Battle slot for the creature (B1, ..., B7)\n"
  "\n"
  "- board\n"
  "    Toggles the board printing.\n"
  "\n"
  "- creature <HAND_CARD_ID> <FIELD_SLOT>\n"
  "    Places a creature from your hand into a Field Zone slot.\n"
  "    <HAND_CARD_ID>: The ID of the creature card in your hand\n"
  "    <FIELD_SLOT>: The Field Zone slot to place the creature in\n"
  "\n"
  "- done\n"
  "    Ends your turn for this round.\n"
  "\n"
  "- graveyard\n"
  "    Prints all the cards in your graveyard.\n"
  "\n"
  "- hand\n"
  "    Prints your hand cards.\n"
  "\n"
  "- info <CARD_ID>\n"
  "    Prints card information.\n"
  "    <CARD_ID>: The ID of the card to be inspected\n"
  "\n"
  "- redraw\n"
  "    Discards all hand cards and draws the same amount minus one from your deck.\n"
  "\n"
  "- spell <HAND_CARD_ID> [<TARGET_SLOT>|<GRAVEYARD_CARD_ID>]\n"
  "    Casts a spell from your hand.\n"
  "    <HAND_CARD_ID>: The ID of the spell in your hand\n"
  "    <TARGET_SLOT>: The slot to target with a target spell\n"
  "    <GRAVEYARD_CARD_ID>: The ID of a card in the graveyard to cast a graveyard spell on\n"
  "\n"
  "- status\n"
  "    Prints general information about both players.\n"
  "\n"
  "=========================================================================================\n";
}

void Game::infoCommand(const std::vector<std::string>& parameters)
{
  std::string id = parameters[0];
  if(!CardDatabase::isIdCorrect(id))
  {
    throw InvalidCardException();
  }
  CardData data = *CardDatabase::getInfo(id);

  if (data.type_ == CardType::CREATURE)
  {
    std::cout << Messages::getDescription("D_BORDER_INFO") << std::endl;
    std::cout << data.name_ << " " << "[" << data.id_ << "] ("<< data.mana_cost_ << " mana)" << "\n"
    << "Type: Creature" << "\n"
    << "Base Attack: " << data.base_attack_ << "\n"
    << "Base Health: " << data.base_health_ << "\n"
    << "Base Traits: ";
    if (data.traits_.size() == 0)
    {
      std::cout << "-";
    }
    else
    {
      for (size_t i = 0; i < data.traits_.size(); ++i)
      {
        std::cout << CardDatabase::traitToString(data.traits_[i]);
        if (i != data.traits_.size() - 1)
        {
          std::cout << ", ";
        }
      }
    }
    std::cout << std::endl;
    std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
  }
  else
  {
    std::string message = "D_" + data.id_;
    message = Messages::getDescription(message);
    std::cout << Messages::getDescription("D_BORDER_INFO") << std::endl;
    std::cout << data.name_ << " " << "[" << data.id_ << "] (";
    if (data.mana_cost_ == 0)
    {
      std::cout << "XX";
    }
    else
    {
      std::cout << data.mana_cost_;
    }
    std::cout << " mana)" << "\n" << "Type: Spell\n" << "Effect: " << message << std::endl;
    std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
  }
  return;
}

void Game::graveyardCommand()
{
  std::cout << Messages::getDescription("D_BORDER_GRAVEYARD") << std::endl;

  for (int i = active_player_->getGraveyardSize() - 1; i >= 0; i--)
  {
    auto& card = active_player_->getGraveyard().at(i);
    std::cout << card->getId() << " | " << card->getName() << std::endl;
  }

  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
}

void Game::boardCommand()
{
  board_printing_ = !board_printing_;
  if (board_printing_)
  {
    board_.printBoard(players_);
  }
}

void Game::handCommand()
{
  std::cout << Messages::getDescription("D_BORDER_HAND") << std::endl;

  const int slots_in_row = 7;
  const int card_height = 4;

  std::vector<std::shared_ptr<Card>> cards = active_player_->getHand();
  const int slots = cards.size();
  const int rows = (slots + slots_in_row - 1) / slots_in_row;
  const int input_height = card_height * rows;

  std::vector<std::string> output(input_height, " ");

  for (int row_block = 0; row_block < rows; row_block++)
  {
    for (int row = 0; row < card_height; ++row)
    {
      for (int slot = 0; slot < slots_in_row; ++slot)
      {
        int card_index = row_block * slots_in_row + slot;
        if (card_index == slots)
        {
          break;
        }
        output[row + row_block * card_height] += "   ";
        std::shared_ptr<Card> card = cards[card_index];

        output[row + row_block * card_height] += card->getCardRowString(row);
      }
    }
  }

  for (const auto& line : output)
  {
    std::cout << line << std::endl;
  }

  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
}

void Game::redrawCommand()
{
  if (!active_player_->getRedrawStatus())
  {
    throw RedrawDisableException();
  }

  if (active_player_->getHand().size() < 2)
  {
    throw RedrawNotEnoughCardsException();
  }

  int hand_size = static_cast<int>(active_player_->getHand().size());
  int cards_to_draw = hand_size - 1;

  for (auto card : active_player_->getHand())
  {
    active_player_->addCardToDeck(card);
  }
  active_player_->clearHand();

  active_player_->drawCards(cards_to_draw);
}

bool isValidSlot(const std::string& slot_id)
{
  size_t length = slot_id.length();

  if (length == 2)
  {
    return (slot_id[0] == 'F' || slot_id[0] == 'B') &&
      (slot_id[1] >= '1' && slot_id[1] <= '7');
  }
  else if (length == 3)
  {
    return slot_id[0] == 'O' &&
      (slot_id[1] == 'F' || slot_id[1] == 'B') &&
      (slot_id[2] >= '1' && slot_id[2] <= '7');
  }
  return false;
}

void Game::validateCreatureParameters(const std::vector<std::string>& parameters) const
{
  const std::string& card_id = parameters[0];
  const std::string& slot_str = parameters[1];

  if (!CardDatabase::isIdCorrect(card_id))
  {
    throw InvalidCardException();
  }
  if (!isValidSlot(slot_str))
  {
    throw InvalidSlotException();
  }
  if (!(active_player_->isCardInHand(card_id)))
  {
    throw NotInHandException();
  }
  if (CardDatabase::getInfo(card_id)->type_ != CardType::CREATURE)
  {
    throw NotCreatureException();
  }
  if (slot_str[0] != 'F')
  {
    throw NotInFieldException();
  }

  int slot_id;
  Utils::decimalStringToInt(slot_str.substr(1), slot_id);
  if (!board_.isFieldSlotEmpty(active_player_, slot_id))
  {
    throw FieldOccupiedException();
  }
  if (CardDatabase::getInfo(card_id)->mana_cost_ > active_player_->getManaPool().getCurrentMana())
  {
    throw NotEnoughManaException();
  }
}

void Game::creatureCommand(const std::vector<std::string>& parameters)
{
  validateCreatureParameters(parameters);

  active_player_->disableRedrow();

  int slot_id;
  Utils::decimalStringToInt(parameters[1].substr(1), slot_id);

  std::shared_ptr<Card> card = active_player_->removeCardFromHand(parameters[0]);
  std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(card);

  creature->resetCard();
  board_.addCardToFieldZone(active_player_, creature, slot_id);

  creature->lockCard();

  int mana_cost = CardDatabase::getInfo(parameters[0])->mana_cost_;
  active_player_->getManaPool().spend(mana_cost);

  std::cout << "[INFO] " << Messages::getInfo("I_" + parameters[0]) << std::endl;
}

void Game::validateBattlleParameters(const std::vector<std::string>& parameters) const
{
  const std::string& field_slot = parameters[0];
  const std::string& battle_slot = parameters[1];
  if (!isValidSlot(field_slot) || !isValidSlot(battle_slot))
  {
    throw InvalidSlotException();
  }
  if (field_slot[0] != 'F')
  {
    throw NotInFieldException();
  }
  int field_slot_id;
  Utils::decimalStringToInt(field_slot.substr(1), field_slot_id);
  if(board_.isFieldSlotEmpty(active_player_, field_slot_id))
  {
    throw FieldEmptyException();
  }

  std::shared_ptr<Card> card = board_.getFieldZoneSlot(active_player_, field_slot_id);
  std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(card);
  if (creature->isLocked() && !creature->hasTrait(Trait::HASTE))
  {
    throw CreatureCannotBattleException();
  }
  if (battle_slot[0] != 'B')
  {
    throw NotInBattleException();
  }

  int battle_slot_id;
  Utils::decimalStringToInt(battle_slot.substr(1), battle_slot_id);
  if (!board_.isBattleSlotEmpty(active_player_, battle_slot_id))
  {
    throw BattleOccupiedException();
  }
}

void Game::handleChallenger(int battle_slot_id)
{
  std::shared_ptr<Player> opponent = (active_player_ == players_[0]) ? players_[1] : players_[0];
  if (!board_.isFieldSlotEmpty(opponent, battle_slot_id) &&
    board_.isBattleSlotEmpty(opponent, battle_slot_id))
  {
    std::shared_ptr<Card> card = board_.getFieldZoneSlot(opponent, battle_slot_id);
    board_.removeCardFromFieldZone(opponent, card);

    board_.addCardToBattleZone(opponent, card, battle_slot_id);
    std::cout << "[INFO] " << Messages::getInfo("I_CHALLENGER") << std::endl;
  }
}

void Game::battleCommand(const std::vector<std::string>& parameters)
{
  validateBattlleParameters(parameters);

  active_player_->disableRedrow();

  int field_slot_id;
  Utils::decimalStringToInt(parameters[0].substr(1), field_slot_id);

  std::shared_ptr<Card> card = board_.getFieldZoneSlot(active_player_, field_slot_id);
  board_.removeCardFromFieldZone(active_player_, card);

  int battle_slot_id;
  Utils::decimalStringToInt(parameters[1].substr(1), battle_slot_id);

  board_.addCardToBattleZone(active_player_, card, battle_slot_id);

  std::shared_ptr<Creature> creature = std::dynamic_pointer_cast<Creature>(card);
  if (creature->isLocked())
  {
    std::cout << "[INFO] " << Messages::getInfo("I_HASTE") << std::endl;
  }
  if (creature->hasTrait(Trait::CHALLENGER))
  {
    handleChallenger(battle_slot_id);
  }
}

void Game::statusCommand()
{
  std::cout << Messages::getDescription("D_BORDER_STATUS") << std::endl;
  std::cout << "Player 1" << std::endl;
  players_[0]->printStatus();

  std::cout << Messages::getDescription("D_BORDER_C") << std::endl;
  std::cout << "Player 2" << std::endl;
  players_[1]->printStatus();

  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;
}

void Game::validateSpellParameters(const std::vector<std::string>& parameters)
{
  std::string spell_id = parameters[0];
  if (parameters.size() == 0)
  {
    throw MissingCardException();
  }
  if (!CardDatabase::isIdCorrect(spell_id))
  {
    throw InvalidCardException();
  }
  std::vector<std::shared_ptr<Card>> hand_cards = (active_player_->getHand());
  if (!(active_player_->isCardInHand(spell_id)))
  {
    throw NotInHandException();
  }
  if (CardDatabase::getInfo(spell_id)->type_ != CardType::SPELL)
  {
    throw NotSpellException();
  }
  std::shared_ptr<Card> spell;
  for (const auto& c : hand_cards)
  {
    if ((c->getId()) == spell_id)
    {
      spell = c;
      break;
    }
  }
  std::shared_ptr<Spell> spell_conv = std::dynamic_pointer_cast<Spell>(spell);
  SpellType type = spell_conv->getType();
  switch (type)
  {
    case SpellType::GENERAL:
      if (parameters.size() != 1)
      {
        throw InvalidParamCountSpellException();
      }
      break;
    case SpellType::GRAVEYARD:
      if (parameters.size() != 2)
      {
        throw InvalidParamCountSpellException();
      }
      break;
    case SpellType::TARGET:
      if (parameters.size() != 2)
      {
        throw InvalidParamCountSpellException();
      }
      break;
    default:
      break;
  }
  if ((CardDatabase::getInfo(spell_id)->mana_cost_) > ((active_player_->getManaPool()).getCurrentMana()))
  {
    throw NotEnoughManaException();
  }
}

void Game::spellCommand(const std::vector<std::string>& parameters)
{
  validateSpellParameters(parameters);

  active_player_->disableRedrow();

  std::string id = parameters[0];
  const std::vector<std::shared_ptr<Card>> hand_search = active_player_->getHand();
  std::shared_ptr<Card> card;
  for (const auto& c : hand_search)
  {
    if (id == c->getId())
    {
      card = c;
      break;
    }
  }
  std::shared_ptr<Spell> converted_spell = std::dynamic_pointer_cast<Spell>(card);
  try
  {
    converted_spell->executeSpell(parameters);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what();
    return;
  }

  int mana_cost = converted_spell -> getManaCost();
  active_player_->getManaPool().spend(mana_cost);
  card = active_player_->removeCardFromHand(id);

  std::cout << "[INFO] " << Messages::getInfo("I_" + parameters[0]) << std::endl;
  board_.cleanupDeadCreatures(players_);
}

void Game::applyRegenerateIfNeeded(std::shared_ptr<Creature> creature)
{
  if (creature->hasTrait(Trait::REGENERATE) && current_round_ % 2 == 1 &&
    creature->getCurrentHealth() < creature->getBaseHealth())
  {
    creature->regenerate();
    std::cout << "[INFO] " << Messages::getInfo("I_REGENERATE") << std::endl;
  }
}

void Game::applyPoisonedIfNeeded(std::shared_ptr<Creature> creature)
{
  if (creature->hasTrait(Trait::POISONED))
  {
    creature->takeDamage(1);
    std::cout << "[INFO] " << Messages::getInfo("I_POISONED") << std::endl;
  }
  if (creature->getCurrentHealth() <= 0)
  {
    board_.moveToGraveyard(active_player_, creature);
  }
}

void Game::applyTraitsInZone(const std::vector<std::shared_ptr<Card>>& zone)
{
  for (auto& card : zone)
  {
    if (card)
    {
      auto creature = std::dynamic_pointer_cast<Creature>(card);

      applyRegenerateIfNeeded(creature);
      applyPoisonedIfNeeded(creature);
    }
  }
}

void Game::handleEffects()
{
  auto& battle_zone = Board::getPlayerBattleZone(active_player_);
  auto& field_zone = Board::getPlayerFieldZone(active_player_);

  applyTraitsInZone(battle_zone);
  applyTraitsInZone(field_zone);
}

int Game::doneCommand()
{
  handleEffects();

  active_player_->disableRedrow();

  if (board_printing_)
  {
    board_.printBoard(players_);
  }

  if (active_player_->getRole() == Role::ATTACKER)
  {
    active_player_ = (active_player_ == players_[0]) ? players_[1] : players_[0];
    return 0;
  }
  else
  {
    battlePhase();
    if (current_phase_ != Phase::FINAL)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
}

int Game::executeCommand(const Command& cmd)
{
  switch (cmd.getType())
  {
    case CommandType::HELP:
      helpCommand();
      return 0;
    case CommandType:: GRAVEYARD:
      graveyardCommand();
      return 0;
    case CommandType::BOARD:
      boardCommand();
      return 0;
    case CommandType::INFO:
      infoCommand(cmd.getParameters());
      return 0;
    case CommandType::REDRAW:
      redrawCommand();
      return 0;
    case CommandType::SPELL:
      spellCommand(cmd.getParameters());
      return 0;
    case CommandType::STATUS:
      statusCommand();
      return 0;
    case CommandType::DONE:
      return doneCommand();
    case CommandType::BATTLE:
      battleCommand(cmd.getParameters());
      return 0;
    case CommandType::HAND:
      handCommand();
      return 0;
    case CommandType::CREATURE:
      creatureCommand(cmd.getParameters());
      return 0;
    default:
      return 0;
  }
}

void Game::battlePhase()
{
  std::cout << "\n" << Messages::getDescription("D_BORDER_BATTLE_PHASE") << std::endl;

  for (int slot = 1; slot <= 7; ++slot)
  {
    std::cout << "---------------------------------------- SLOT " << slot
      << " -----------------------------------------\n";
    resolveSlot(slot);
    if (current_phase_ == Phase::FINAL) return;
  }

  std::cout << Messages::getDescription("D_BORDER_BATTLE_END") << std::endl;
  board_.removeTemporary(players_);
  board_.returnToFieldZone(players_);
  board_.handleUndying(players_);
}

void Game::endGame(GameEndReason reason)
{
  std::cout << "\n" << Messages::getDescription("D_BORDER_GAME_END") << std::endl;

  std::string end_msg;
  switch (reason)
  {
    case GameEndReason::PLAYER_DEFEATED:
      end_msg = Messages::getDescription("D_END_PLAYER_DEFEATED");
      break;
    case GameEndReason::DRAW_CARD_FAILED:
      end_msg = Messages::getDescription("D_END_DRAW_CARD");
      break;
    case GameEndReason::MAX_ROUNDS_EXCEEDED:
      end_msg = Messages::getDescription("D_END_MAX_ROUNDS");
      break;
  }
  std::cout << end_msg << std::endl;

  std::string result;

  if (players_[0]->isWinner() && players_[1]->isWinner())
  {
    result = Messages::getDescription("D_TIE") + "\n";
  }
  else if (players_[0]->isWinner())
  {
    result = "Player 1 has won! Congratulations!\n";
  }
  else
  {
    result = "Player 2 has won! Congratulations!\n";
  }

  std::cout << result;
  std::cout << Messages::getDescription("D_BORDER_D") << std::endl;

  std::ofstream file(config_file_path_, std::ios::app);
  if (file.is_open())
  {
    file << "\n" << result;
    file.close();
  }
  else
  {
    std::cout << Messages::getDescription("I_FILE_WRITE_FAILED") << std::endl;
  }
}

void Game::resolveSlot(int slot_id)
{
  auto defender = (players_[0]->getRole() == Role::DEFENDER) ? players_[0] : players_[1];
  auto attacker = (players_[0]->getRole() == Role::ATTACKER) ? players_[0] : players_[1];
  auto attack_card = board_.getBattleZoneSlot(attacker, slot_id);
  if (!attack_card)
  {
    return;
  }
  auto attack_creature = std::dynamic_pointer_cast<Creature>(attack_card);
  auto defence_card = board_.getBattleZoneSlot(defender, slot_id);
  if (!defence_card)
  {
    directHit(attack_creature, defender);
    if (current_phase_ == Phase::FINAL)
    {
      attacker->makeWinner();
      endGame(GameEndReason::PLAYER_DEFEATED);
    }
  }
  else
  {
    auto defence_creature = std::dynamic_pointer_cast<Creature>(defence_card);
    if (defence_creature)
    {
      fightCreatures(attack_creature, defence_creature, attacker, defender);
    }
  }
}

void Game::directHit(std::shared_ptr<Creature> attack_card, std::shared_ptr<Player> defender)
{
  std::cout << "[INFO] " << Messages::getInfo("I_DIRECT") << std::endl;
  defender->takeDamage(attack_card->getCurrentAttack());
  if (defender->getHealth() <= 0)
  {
    current_phase_ = Phase::FINAL;
  }
}

void Game::handleAttack(std::shared_ptr<Creature> attack_creature, std::shared_ptr<Creature> defence_creature,
  std::shared_ptr<Player> defender)
{
  defence_creature->takeDamage(attack_creature->getCurrentAttack());

  if (attack_creature->hasTrait(Trait::BRUTAL) && defence_creature->getCurrentHealth() <= 0)
  {
    std::cout << "[INFO] " << Messages::getInfo("I_BRUTAL") << std::endl;
    defender->takeDamage(-defence_creature->getCurrentHealth());
  }
  if (attack_creature->hasTrait(Trait::LIFESTEAL))
  {
    attack_creature->heal(2);
    std::cout << "[INFO] " << Messages::getInfo("I_LIFESTEAL") << std::endl;
  }
  if (attack_creature->hasTrait(Trait::VENOMOUS))
  {
    std::cout << "[INFO] " << Messages::getInfo("I_VENOMOUS") << std::endl;
    defence_creature->addTrait(Trait::POISONED);
  }
}

void Game::fightCreatures(std::shared_ptr<Creature> attack_creature, std::shared_ptr<Creature> defence_creature,
  std::shared_ptr<Player> attacker, std::shared_ptr<Player> defender)
{
  std::cout << "[INFO] " << Messages::getInfo("I_FIGHT") << std::endl;

  bool first_strike = false;
  auto first_attack_card = attack_creature;
  auto second_attack_card = defence_creature;
  auto first_defender = defender;
  auto second_defender = attacker;

  std::cout << Messages::getDescription("D_ATTACK_1") << std::endl;

  if (attack_creature->hasTrait(Trait::FIRST_STRIKE) && !defence_creature->hasTrait(Trait::FIRST_STRIKE))
  {
    std::cout << "[INFO] " << Messages::getInfo("I_FIRST_STRIKE") << std::endl;
    first_strike = true;
  }
  else if (!attack_creature->hasTrait(Trait::FIRST_STRIKE) && defence_creature->hasTrait(Trait::FIRST_STRIKE))
  {
    std::cout << "[INFO] " << Messages::getInfo("I_FIRST_STRIKE") << std::endl;
    first_attack_card = defence_creature;
    second_attack_card = attack_creature;
    first_defender = attacker;
    second_defender = defender;
    first_strike = true;
  }

  handleAttack(first_attack_card, second_attack_card, first_defender);

  if (!(first_strike && second_attack_card->getCurrentHealth() <= 0))
  {
    std::cout << Messages::getDescription("D_ATTACK_2") << std::endl;
    handleAttack(second_attack_card, first_attack_card, second_defender);
  }

  board_.cleanupDeadCreatures(players_);

  bool is_attacker_dead = (attacker->getHealth() <= 0) ? true : false;
  if (is_attacker_dead)
  {
    defender->makeWinner();
  }

  bool is_defender_dead = (defender->getHealth() <= 0) ? true : false;
  if (is_defender_dead)
  {
    attacker->makeWinner();
  }

  if (is_attacker_dead || is_defender_dead)
  {
    current_phase_ = Phase::FINAL;
    endGame(GameEndReason::PLAYER_DEFEATED);
  }
}

int Game::makeGame()
{
  CommandLine cli;
  bool game_status = true;

  while (game_status)
  {
    try
    {
      game_status = setRound();

      while (game_status)
      {
        try
        {
          playerLog();
          Command command = cli.readCommand();
          if (command.isQuit())
          {
            game_status = false;
            break;
          }
          int command_status = executeCommand(command);
          if (command.getType() == CommandType::DONE)
          {
            if (command_status == 1)
            {
              break;
            }
            else if (command_status == 2)
            {
              game_status = false;
              break;
            }
          }
        }
        catch (const GameError& exception)
        {
          std::cout << exception.what();
        }
      }
    }
    catch (const std::bad_alloc& exeption)
    {
      std::cout << "[ERROR] Not enough memory!\n";
      return 0;
    }
  }
  return 1;
}

int Game::getCurrentRound() const
{
  return current_round_;
}

Phase Game::getCurrentPhase() const
{
  return current_phase_;
}

void Game::playerLog()
{
  std::cout << ((players_[0] == active_player_) ? "\nP1> " : "\nP2> ");
}

std::vector<std::shared_ptr<Player>> Game::getPlayers() const
{
  return players_;
}

const Board& Game::getBoard() const
{
  return board_;
}
