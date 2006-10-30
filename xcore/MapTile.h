#ifndef XCORE_MAPTILE_H
#define XCORE_MAPTILE_H

#include "includes.h"

namespace xcore 
{
   class MapTile;
   typedef boost::shared_ptr<MapTile> spMapTile;
   typedef boost::weak_ptr<MapTile> wpMapTile;
}

#include "Point.h"
#include "Terrain.h"
#include "MapObject.h"
#include "Direction.h"
#include "Unit.h"

namespace xcore
{

/// A single tile on the map
class MapTile
{
   public:
      static spMapTile makeMapTile(TerrainType type, int x, int y);
      virtual ~MapTile();

      bool isPassable() const;
      bool isPassable(bool ignoreObjects) const;
      bool isPassable(const Direction& dir, bool ignoreObject=false) const;
      bool hasObstruction(const Direction& dir) const;
      int getX() const;
      int getY() const;

      virtual void draw();

      /// The x coordinate for the top left drawing point
      /// @todo Move this out of here to an external sprite class or resource handler
      virtual int getScreenX() const;
      /// The y coordinate for the top left drawing point
      /// @todo Move this out of here to an external sprite class or resource handler
      virtual int getScreenY() const;
      /// The x coordinate for the center of the tile
      /// @todo Move this out of here to an external sprite class or resource handler
      virtual int getCenterX() const;
      /// The y coordinate for the center of the tile
      /// @todo Move this out of here to an external sprite class or resource handler
      virtual int getCenterY() const;

      virtual spTerrain makeTerrain(TerrainType type);
      TerrainType getTerrainType();

      /// Cost in action points to move in that direction
      /// @todo This should really be in Unit because this can vary on a per unit basis
      int getMoveCost(const Direction& dir) const;

      /// Adds a reference to the tile in the specified direction and adds a reference
      /// to this tile in the reverse direction for that tile
      void addTileInDirection( spMapTile tile, const Direction& dir);
      spMapTile getTileInDirection( const Direction& dir ) const;

      /// Places a unit on the tile
      /// @todo Should move this into an association class
      bool addUnit(spUnit unit);
      spUnit getUnit() const;    // explicitly non-virtual
      void removeUnit();
      bool hasUnit() const;

      bool addObject(spMapObject object);
      void removeObject(spMapObject object);

      virtual void drawTerrain(Point offset) const;
      virtual void drawObjects(Point offset) const;
      virtual void highlight(Point offset);
      virtual void highlightMoveable(Point offset);
        // passes on damage from explosions, etc.
        void takeDamage( int damage );
        // gets the distance between two tiles
        int getDistance( spMapTile tile );

   protected:
      MapTile(int x, int y);
      MapTile(TerrainType type, int x, int y);

      template <class T>
      static boost::shared_ptr<T> makeMapTileHelper(TerrainType type, int x, int y);

      spTerrain getTerrain() const;
      void setTerrain(spTerrain terrain);

      list<spMapObject> _objects;
      spUnit _unit;

   private:
      /// Used when the tile needs to pass the 'this' pointer to something that only
      /// accepts smart pointers.
      wpMapTile _weak;

      uint32 _x;
      uint32 _y;

      spTerrain _terrain;      

      map<Direction, spMapTile> directionTileMap;

      static uint16 cardinalMoveBase;
      static uint16 diagonalMoveBase;
};

template <class T>
boost::shared_ptr<T> MapTile::makeMapTileHelper(TerrainType type, int x, int y)
{
   boost::shared_ptr<T> tile(new T(type, x, y));
   tile->_weak = tile;
   return tile;
}

} // namespace

#endif

