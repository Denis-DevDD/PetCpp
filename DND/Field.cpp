



#include "Field.hpp"
#include <iostream>
#include <string>
#include "Entity.hpp"
#include "Enemy.hpp"
std::ostream& operator<<(std::ostream& stream, const Field& field)
{
  std::string output = "   ";
  if (field.current_entity_ == nullptr)
  {

  }
  else
  {
    if (field.current_entity_->getId() == "Z")
    {
      output = " Z" + std::to_string(field.current_entity_->getNumber());
    }
    if (field.current_entity_->getId() == "D")
    {
      output = "#D" + std::to_string(field.current_entity_->getNumber());
      if (field.current_entity_->getIfLocked() == false)
      {
        output = " D" + std::to_string(field.current_entity_->getNumber());
      }
    }
      if (field.current_entity_->getId() == "G")
    {
      output = " G"  + std::to_string(field.current_entity_->getNumber());
    }
      if (field.current_entity_->getId() == "T")
    {
      output = "#T ";
      if (field.current_entity_->getIfLocked() == false)
      {
        output = " T ";
      }
    }
      if (field.current_entity_->getId() == "W")
    {
      output = " W ";
    }
      if (field.current_entity_->getId() == "B")
    {
      output = " B ";
    }
      if (field.current_entity_->getId() == "R")
    {
      output = " R ";
    }
      if (field.current_entity_->getId() == "L")
    {
      output = " L"+ std::to_string(field.current_entity_->getNumber());
    } if (field.current_entity_->getId() == "X")
    {
      output = " X ";
    }


  }

  stream<<""<<output<<"|";
  return stream;
}
void Field::setEntity(Entity *entity)
{
  current_entity_ = entity;
}
Entity* Field::getEntity()
{
  return current_entity_;
}
Field::Field(int x, int y)
{
  posX_ = x;
  posY_ = y;
  current_entity_ = nullptr;
}
int Field::getX()
{
  return posX_;
}
int Field::getY()
{
  return posY_;
}
Field::~Field()
{
  if (current_entity_ != nullptr)
  {
    if(current_entity_->getId()!="W" && current_entity_->getId()!="B" && current_entity_->getId()!="R")
    {
      if (!current_entity_->getIfplayer())
      {
        delete current_entity_;
      }
      current_entity_ = nullptr;
    }
  }
}
