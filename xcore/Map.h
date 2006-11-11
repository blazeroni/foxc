#ifndef XCORE_MAP_H
#define XCORE_MAP_H

#include "includes.h"
#include "MapTile.h"
#include "Terrain.h"
#include "EntityFactory.h"

namespace xcore
{

class Map;
typedef boost::shared_ptr<Map> spMap;
typedef boost::weak_ptr<Map> wpMap;

class Map
{
   public:
      // sending the factory in is kludge.. need a better way to do it, but time is short
      static spMap makeMap(spEntityFactory factory);
      virtual ~Map();

      bool load( string fileName );
      bool load( istream& data );

      virtual void drawTerrainLayer(const Point& offset);
      virtual void drawObjectLayer(const Point& offset);
      //void drawMinimap();
      virtual void highlightMouseOverTile(const Point& offset);
      
      int getWidth() const;
      int getHeight() const;

      spMapTile getTile(int x, int y) const;    // explicitly non-virtual

      virtual spMapTile getMouseOverTile() const;
      virtual void updateMouseOverTile(const Point& mouse, const Point& offset);

      string getName() const;

   protected:
      Map(spEntityFactory factory);

      void loadWalls(map<spMapTile, bool> walls);

      template <class T>
      static boost::shared_ptr<T> makeMapHelper(spEntityFactory factory);

      virtual spMapTile makeMapTile(TerrainType type, int x, int y);
      //virtual spWall makeWall();

      /// Used when the map needs to pass the 'this' pointer to something that only
      /// accepts smart pointers.
      wpMap _weak;

      vector<spMapTile> _mapTiles;
      spEntityFactory _factory;

   private:
      spMapTile getTile(Point position, Point offset);
      bool makeTerrain(string terrain);

      void populateTileNeighbors(spMapTile tile);

      string _name;
      int _width;
      int _height;
      
      static map<string, TerrainType> _terrainMap;
      //static map<string, MapEdgeObject> _edgeMap;
      
      /**
       * Mouse map image to determine which tile the mouse is pointing at
       *
       * Each tile is a different color, and depending on the color the tile is 
       * determined.  Used instead of complicated equations.
       */
//      SDL_Surface* _tileMouseMap;
      //spMapTile _mouseOverTile;
};

template <class T>
boost::shared_ptr<T> Map::makeMapHelper(spEntityFactory factory)
{
   boost::shared_ptr<T> map(new T(factory));
   map->_weak = map;
   return map;
}

} // namespace

#endif

