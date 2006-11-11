#ifndef XCORE_UNIT_H
#define XCORE_UNIT_H

#include "includes.h"

#define COST_FIRE 25

namespace xcore
{
   class Unit;
   typedef boost::shared_ptr<Unit> spUnit;
   typedef boost::weak_ptr<Unit> wpUnit;
}

#include "Item.h"
#include "Point.h"
#include "GameEntity.h"
#include "MapTile.h"

namespace xcore
{

/**
 * A player-controlled unit
 *
 * 
 */
class Unit : public GameEntity
{
   public:
      static spUnit makeUnit(uint32 entityID, int playerID);
      static spUnit makeUnit(uint32 entityID, int playerID, spMapTile);
      virtual ~Unit();

      int getPlayerID() const;
      int getX() const;
      int getY() const;

      virtual void draw(const Point& offset, const Point& dimensions) const;

      bool isPassable() const;
 
      void wait();

      virtual void move( const Direction& dir );
      virtual void move( spMapTile tile );

      /// Moves the unit one square along the path, unless teleport
      /// is true - then it goes to the destination
      void moveAlongPath( bool teleport = false );

      /// Retrieves the current possible moves.  This does not actually
      /// compute the moves, so it can be called often
      map<spMapTile, int> getPossibleMoves();

      /// Compute the possible moves for this character.  Should only
      /// be called when something changes in the game
      void updatePossibleMoves();
      void clearPossibleMoves();
      bool canMoveInDirection(const Direction& dir);

      /// Creates a the path to the destination square
      void computeMovePath(spMapTile dest);
      virtual void drawMovePath(const Point& offset) const;
      bool hasMovePath() const;
      void clearMovePath();

      /// Returns the ending MapTile for the unit's movement path
      const spMapTile getDestination() const;

      /// returns number of action points
      int getActionPoints() const;
      bool hasMaxActionPoints() const;
      void regenActionPoints(int times = 1);
      void useActionPoints(int points);

      int getHealth() const;

      /**
       * Provides a strict weak ordering comparison, i.e. "less than"
       * 
       * Created for STL comparisons, but potentially useful elsewhere.
       * If they are equal, this will return false but really it's undefined
       * since the order in the queue would actually take priority (which is arbitrary)
       */
      bool hasTurnBefore(spUnit u) const;
     
      void markActive();
      static void clearActive();

      virtual void markSelected();

      /// setStats takes the unit's job and rank and generates the stats for the unit
      void setStats();
   
      /// nameUnit takes in a string and applies it to the unit.
      // CURRENTLY NOT IMPLEMENTED
      void nameUnit(string name);

      /// shoot at a tile
      void fire(spMapTile tile);
      /// take damage from something
      void takeDamage( int damage );
      /// die
      void die();
      /// heal from medkit
      void heal();

      /// equips an item
      bool equip( spItem item );
      /// drops an item
      bool drop( spItem item );
      /// moves right hand item to inventory
      bool unequipRight();
      /// moves left hand item to inventory
      bool unequipLeft();
      /// returns the accuracy of the unit
      int getAccuracy() const;
      /// returns the rank of the current unit
      int getRank() const;
	  // Add an item to the unit's inventory
	  bool addItem(spItem item);
	  // Return the unit's inventory
	  spItem* getInventory();

      virtual vector<string> getResourceIDs();

    void swapEq( int slot1, int slot2 );
    enum hand { RIGHT = 0, LEFT = 1 };
    void use( spMapTile tile, int wHand  = 0);
    void useLeft( spMapTile tile = spMapTile() );
    void useRight( spMapTile tile = spMapTile() );
    SDL_Surface* getImage();
    void reload();
    spItem getInv( int slot );
    spItem getHand( int h );
    spItem getLeft();
    spItem getRight();
    string getName() const;

   protected:
      Unit(uint32 enittyID, int playerID, spMapTile tile);

      template <class T>
      static boost::shared_ptr<T> makeUnitHelper(unsigned long entityID, int playerID, spMapTile tile);

      static spUnit _activeUnit;

      int _health;

      /// Used when the unit needs to pass the 'this' pointer to something that only
      /// accepts smart pointers.
      wpUnit _weak;

      /// The tile the unit is current on
      /// @todo Move this into an association class
      spMapTile _tile;

      map<spMapTile, int> _possibleMoves;
      list<pair<spMapTile, int> > _movementPath;

   private:
      /// checks if the unit can move from tile in the direction specified with the given amount of points
      /// used mainly in pathing
      bool canMoveInDirection(spMapTile tile, const Direction& dir, int points);
      pair<spMapTile, int> computeMovePathHelper(spMapTile current);

      int _actionPoints;
      int _regenRate;
      int _playerID;

      static int _maxActionPoints;
      static int _maxHealth;
      static int _maxMorale;

      /// inventory slots
      spItem _hand[2];
      spItem _inventory[5];
      /// Job and rank, and unit stats
      int _rank;
      int _accuracy;

      int _morale;
      string _name;
};

template <class T>
boost::shared_ptr<T> Unit::makeUnitHelper(long unsigned int entityID, int playerID, spMapTile tile)
{
   boost::shared_ptr<T> unit(new T(entityID, playerID, tile));
   unit->_weak = unit;
   unit->_tile->addUnit(unit);
   return unit;

   //return boost::shared_ptr<T>();
}

} // namespace

#endif
