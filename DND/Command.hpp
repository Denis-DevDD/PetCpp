#ifndef COMMAND_HPP
#define COMMAND_HPP
class Game;
class Command
{
protected:
  bool if_action_command_;
public:
  Command(bool if_action) {if_action_command_ = if_action;};
  virtual bool execute(Game*) {return true;}

  virtual ~Command() {};

  Command(const Command&) = delete;

  bool getIfAction() {return if_action_command_;}
};
#endif
