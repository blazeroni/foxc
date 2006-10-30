#ifndef XCORE_PLAYER_H
#define XCORE_PLAYER_H

#include "includes.h"
#include "Unit.h"

namespace xcore
{

class Player;
typedef boost::shared_ptr<Player> spPlayer;

class Player
{
  public:
     Player(int id, string name = "");
     ~Player();

     string getName();
     void setName( string name );
     int getID();

     void removeUnit( spUnit unit );
     void addUnit( spUnit unit );
     int getUnitCount();

     //spUnit getUnitIterator();
     //spUnit getNextUnit();

  private:
     int _id;
     string _name;
  //    int color;
  //    int team;

     vector<spUnit> _units;
};

} // namespace

#endif
