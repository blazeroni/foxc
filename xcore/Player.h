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
      Player(uint32 id, string name = "", uint16 num = 0);
      ~Player();

      string getName() const;
      void setName( string name );

      uint16 getPlayerNumber() const;
      void setPlayerNumber(uint16 num);

      uint32 getID() const;

      void removeUnit( spUnit unit );
      void addUnit( spUnit unit );
      int getUnitCount();

   private:
      uint32 _id;        // ID on the server
      string _name;
      uint16 _playerNum;  // number for this game (1 to 8 or whatever)

      vector<spUnit> _units;
};

} // namespace

#endif
