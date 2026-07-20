#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Player.hpp"
#include "Board.hpp"
#include "Command.hpp"
#include "Utils.hpp"
#include "Messages.hpp"
#include "Creature.hpp"
#include "CardCreator.hpp"
#include "GameError.hpp"
#include "CardDatabase.hpp"
#include "CommandLine.hpp"

enum class Phase
{
  SETUP,
  BATTLE,
  FINAL
};

enum class GameEndReason
{
  PLAYER_DEFEATED,
  DRAW_CARD_FAILED,
  MAX_ROUNDS_EXCEEDED
};

class Game
{
  private:
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<Player> active_player_;
    Board board_;
    int current_round_;
    int max_rounds_;
    int initial_mana_pool_size_;
    Phase current_phase_;
    bool board_printing_;
    std::string config_file_path_;

  public:
    Game();
    Game(const Game&) = delete;
    ~Game() = default;

    int makeGame();

    int loadConfigFile(const char* path);
    static bool loadMessages(const char* path);

    void start();
    bool setRound();
    void swapRole();
    void playerLog();
    void resolveSlot(int slot_id);
    void battlePhase();
    void endGame(GameEndReason reason);

    void helpCommand();
    void infoCommand(const std::vector<std::string>& parameters);
    void handCommand();
    int executeCommand(const Command& cmd);
    void graveyardCommand();
    void boardCommand();
    void redrawCommand();
    void statusCommand();
    void creatureCommand(const std::vector<std::string>& parameters);
    void battleCommand(const std::vector<std::string>& parameters);
    void spellCommand(const std::vector<std::string>& parameters);
    int doneCommand();

    void validateCreatureParameters(const std::vector<std::string>& parameters) const;
    void validateBattlleParameters(const std::vector<std::string>& parameters) const;
    void validateSpellParameters(const std::vector<std::string>& parameters);
    void applyTraitsInZone(const std::vector<std::shared_ptr<Card>>& zone);
    void applyRegenerateIfNeeded(std::shared_ptr<Creature> creature);
    void applyPoisonedIfNeeded(std::shared_ptr<Creature> creature);
    void handleEffects();
    void handleChallenger(int battle_slot_id);

    void directHit(std::shared_ptr<Creature> attack_card, std::shared_ptr<Player> defender);
    void fightCreatures(std::shared_ptr<Creature> attack_creature, std::shared_ptr<Creature> defence_creature,
      std::shared_ptr<Player> attacker, std::shared_ptr<Player> defender);
    void handleAttack(std::shared_ptr<Creature> attack_creature, std::shared_ptr<Creature> defence_creature,
      std::shared_ptr<Player> defender);

    const Board& getBoard() const;
    int getCurrentRound() const;
    Phase getCurrentPhase() const;
    std::vector<std::shared_ptr<Player>> getPlayers() const;
};

#endif
