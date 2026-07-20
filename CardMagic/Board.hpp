#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Player.hpp"

enum class ZoneType
{
  BATTLE,
  FIELD
};

class Board
{
  private:
    static std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> field_zone_;
    static std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> battle_zone_;

  public:
    Board() = default;
    Board(const Board&) = delete;
    ~Board() = default;

    void setBoard(const std::vector<std::shared_ptr<Player>>& players);

    void addCardToFieldZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card, int slot_id);
    void addCardToBattleZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card,int slot_id);
    void removeCardFromFieldZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    void removeCardFromBattleZone(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    void removeTemporary(std::vector<std::shared_ptr<Player>> players);
    void returnToFieldZone(std::vector<std::shared_ptr<Player>> players);
    void handleUndying(std::vector<std::shared_ptr<Player>> players);
    void resolveTemporary(std::shared_ptr<Player> player);
    void resolveOther(std::shared_ptr<Player> player);
    void unlockCards(std::vector<std::shared_ptr<Player>> players);
    void moveToGraveyard(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    void cleanupDeadCreatures(std::vector<std::shared_ptr<Player>> players);
    static void addSpellCreature(const std::shared_ptr<Player> own, const std::shared_ptr<Card> creature);

    bool isFieldSlotEmpty(std::shared_ptr<Player> player, int slot_id) const;
    bool isBattleSlotEmpty(std::shared_ptr<Player> player, int slot_id) const;
    void printBoard(std::vector<std::shared_ptr<Player>> players) const;
    void printZone(std::shared_ptr<Player> player, ZoneType zone_type) const;

    const std::shared_ptr<Card>& getFieldZoneSlot(const std::shared_ptr<Player> player, int slot_id) const;
    const std::shared_ptr<Card>& getBattleZoneSlot(const std::shared_ptr<Player> player, int slot_id) const;
    static const std::vector<std::shared_ptr<Card>> getPlayerFieldZone(const std::shared_ptr<Player>& player);
    static const std::vector<std::shared_ptr<Card>> getPlayerBattleZone(const std::shared_ptr<Player>& player);
    static const std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> getFieldZone();
    static const std::map<std::shared_ptr<Player>, std::vector<std::shared_ptr<Card>>> getBattleZone();
};

#endif
