#include "includes.h"
#include "Player.h"

namespace xcore
{

Player::Player(uint32 playerID, string name, uint16 num) :
   _name(name),
   _id(playerID),
   _playerNum(num)
{
}

Player::~Player()
{

}

uint32 Player::getID() const
{
   return _id;
}

string Player::getName() const
{
   return _name;
}

void Player::setName(string name)
{
   _name = name;
}

uint16 Player::getPlayerNumber() const
{
   return _playerNum;
}

void Player::setPlayerNumber(uint16 num)
{
   _playerNum = num;
}

void Player::addUnit(spUnit unit)
{
   _units.push_back(unit);
}

//spUnit Player::getNextUnit()
//{
//    // for now returns the first unit in the unit vector
////    return _units[0];
//}

} // namespace
