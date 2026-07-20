#ifndef GAMEERROR_HPP
#define GAMEERROR_HPP

#include <stdexcept>
#include <format>
#include <string>

#include "Messages.hpp"

class GameError : public std::exception
{
  protected:
    std::string key_;
    std::string message_;

  public:
    GameError() = default;
    GameError(const GameError&) = delete;
    ~GameError() = default;

    GameError(const std::string &key) : key_(key), message_(std::format("[ERROR] {}\n", Messages::getError(key_))) {}
    const char *what() const noexcept
    {
      return message_.c_str();
    }
};

class UnknownCommandException : public GameError
{
  public:
    UnknownCommandException() : GameError("E_UNKNOWN_COMMAND") {}
};

class InvalidParametersCountException : public GameError
{
  public:
    InvalidParametersCountException() : GameError("E_INVALID_PARAM_COUNT") {}
};

class MissingCardException : public GameError
{
  public:
    MissingCardException() : GameError("E_MISSING_CARD") {}
};

class InvalidCardException : public GameError
{
  public:
    InvalidCardException() : GameError("E_INVALID_CARD") {}
};

class InvalidSlotException : public GameError
{
  public:
    InvalidSlotException() : GameError("E_INVALID_SLOT") {}
};

class RedrawDisableException : public GameError
{
  public:
    RedrawDisableException() : GameError("E_REDRAW_DISABLED") {}
};

class RedrawNotEnoughCardsException : public GameError
{
  public:
    RedrawNotEnoughCardsException() : GameError("E_REDRAW_NOT_ENOUGH_CARDS") {}
};

class NotInHandException : public GameError
{
  public:
    NotInHandException() : GameError("E_NOT_IN_HAND") {}
};

class NotCreatureException : public GameError
{
  public:
    NotCreatureException() : GameError("E_NOT_CREATURE") {}
};

class NotSpellException : public GameError
{
  public:
    NotSpellException() : GameError("E_NOT_SPELL") {}
};

class InvalidParamCountSpellException : public GameError
{
  public:
    InvalidParamCountSpellException() : GameError("E_INVALID_PARAM_COUNT_SPELL") {}
};

class FieldEmptyException : public GameError
{
  public:
    FieldEmptyException() : GameError("E_FIELD_EMPTY") {}
};

class CreatureCannotBattleException : public GameError
{
  public:
    CreatureCannotBattleException() : GameError("E_CREATURE_CANNOT_BATTLE") {}
};

class NotInBattleException : public GameError
{
  public:
    NotInBattleException() : GameError("E_NOT_IN_BATTLE") {}
};

class NotInFieldException : public GameError
{
  public:
    NotInFieldException() : GameError("E_NOT_IN_FIELD") {}
};

class BattleOccupiedException : public GameError
{
  public:
    BattleOccupiedException() : GameError("E_BATTLE_OCCUPIED") {}
};

class FieldOccupiedException : public GameError
{
  public:
    FieldOccupiedException() : GameError("E_FIELD_OCCUPIED") {}
};

class InvalidSlotSpell : public GameError
{
  public:
    InvalidSlotSpell() : GameError("E_INVALID_SLOT_SPELL") {}
};

class TargetEmptyException : public GameError
{
  public:
    TargetEmptyException() : GameError("E_TARGET_EMPTY") {}
};

class NotInGraveyardException : public GameError
{
  public:
    NotInGraveyardException() : GameError("E_NOT_IN_GRAVEYARD") {}
};

class NotEnoughManaException : public GameError
{
  public:
    NotEnoughManaException() : GameError("E_NOT_ENOUGH_MANA") {}
};

class InvalidFile : public std::exception
{
  public:
    const char* what() const noexcept override
    {
      return "Invalid file!";
    }
};

#endif
