#include "includes.h"
#include "Player.h"

namespace xcore
{

Player::Player(int playerID, string name) :
   _name(name),
   _id(playerID)
{
}

Player::~Player()
{

}

int Player::getID()
{
   return _id;
}

string Player::getName()
{
   return _name;
}

void Player::setName(string name)
{
   _name = name;
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
