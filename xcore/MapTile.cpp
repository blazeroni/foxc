#include "includes.h"
#include "MapTile.h"
#include "UnsupportedOperationException.h"

namespace xcore
{

//int MapTile::_width = 80;
//int MapTile::_height = 42;
//SDL_Surface* MapTile::_highlight = NULL;
//SDL_Surface* MapTile::_highlightMove = NULL;
uint16 MapTile::cardinalMoveBase = 15;
uint16 MapTile::diagonalMoveBase = 10;

//#define TILE_HIGHLIGHT "images/tile_highlight.png"
//#define TILE_MOVEABLE "images/tile_moveable.png"

spMapTile MapTile::makeMapTile(TerrainType type, int x, int y)
{
   return makeMapTileHelper<MapTile>(type, x, y);
   //spMapTile tile(new MapTile(type, x, y));
   //tile->_weak = tile;
   //return tile;
}

MapTile::MapTile(int x, int y) :
   _x(x),
   _y(y),
   _unit(spUnit()),
   _hasDebris(false)
{

}

MapTile::MapTile(TerrainType type, int x, int y) :
   _x(x),
   _y(y),
   _terrain(makeTerrain(type)),
   _unit(spUnit())
{
   // @todo: change _terrain later so each MapTile doesn't have it's own Terrain;
   //static halfHeight = MapTile::_height / 2;
    //_screenX = MapTile::_width/2*_map->getHeight() + y*MapTile::_width/2 - (x)*MapTile::_width/2;
//    _screenX = y*MapTile::_width/2 - (x)*MapTile::_width/2;
//    _screenY = (y+x) * MapTile::_height/2;
    //_centerX = MapTile::_width/2*_map->getHeight() + y*MapTile::_width/2 - (x-1)*MapTile::_width/2;
//    _centerX = y*MapTile::_width/2 - (x-1)*MapTile::_width/2;
//    _centerY = (y+x+1) * MapTile::_height/2;

   //if (!_highlight)
   //{
   //   _highlight = Display::instance().loadImage(TILE_HIGHLIGHT);
   //   _highlightMove = Display::instance().loadImage(TILE_MOVEABLE);
   //}
}

MapTile::~MapTile()
{
}

spTerrain MapTile::makeTerrain(TerrainType type) {
   return spTerrain(new Terrain(type));
}

int MapTile::getX() const
{
   return _x;
}
int MapTile::getY() const
{
   return _y;
}
int MapTile::getScreenX() const
{
   throw UnsupportedOperationException();
}
int MapTile::getScreenY() const
{
   throw UnsupportedOperationException();
}
int MapTile::getCenterX() const
{
   throw UnsupportedOperationException();
}
int MapTile::getCenterY() const
{
   throw UnsupportedOperationException();
}
TerrainType MapTile::getTerrainType()
{
    return _terrain->getType();
}

void MapTile::draw()
{
   throw UnsupportedOperationException();
}

void MapTile::drawObjects(Point offset) const
{
   throw UnsupportedOperationException();
}

void MapTile::highlight(Point offset)
{
   throw UnsupportedOperationException();
}

void MapTile::highlightMoveable(Point offset)
{
   throw UnsupportedOperationException();
}

bool MapTile::isPassable() const
{
   return isPassable(false);
}

bool MapTile::isPassable(bool ignoreObjects) const
{
   bool passable = true;
   if (_terrain->isPassable())
   {
      if (!ignoreObjects && !hasUnit())
      {
         list<spMapObject>::const_iterator iter;
         for (iter = _objects.begin(); iter != _objects.end(); ++iter)
         {
            if (!(*iter)->isPassable())
            {
               passable = false;
               break;
            }
         }
      }
      //find_if(_objects.begin(), _objects.end(), not1(mem_fun(&MapObject::isPassable)));
   }
   else
   {
      passable = false;
   }

   return passable;
}
/*bool MapTile::isPassable(const Direction& dir, bool ignoreObjects) const
{
    spMapTile t = getTileInDirection(dir);
    if ( !t.get() )
    {
       return false;
    }
   
    return (isPassable(ignoreObjects) && !hasObstruction(dir) && t->isPassable(ignoreObjects));
}*/

/*bool MapTile::hasObstruction(const Direction& dir) const
{
   if (!_objects.empty())
   {
        list<spMapObject>::const_iterator iter;
        for (iter = _objects.begin(); iter != _objects.end(); ++iter)
        {
            if ( !(*iter)->isPassable(dir) )
                return true;
        }
   }
   return false;
}*/

bool MapTile::hasUnit() const
{
   return _unit.get() != NULL;
}

bool MapTile::addUnit(spUnit unit)
{
   if (isPassable())
   {
      _unit = unit;
      return true;
   }
   else
   {
      return  false;
   }
}

spUnit MapTile::getUnit() const
{
   return _unit;
}

void MapTile::removeUnit()
{
   _unit = spUnit();
}

bool MapTile::addObject(spMapObject object)
{
    /*
   if (object->isPassable() || isPassable())
   {
      _objects.push_back(object);
      return true;
   }
   return false;
   */
    _objects.push_back(object);
    return true;
}

void MapTile::removeObject(spMapObject object)
{
   _objects.remove(object);
}

void MapTile::addTileInDirection( spMapTile tile, const Direction& dir)
{
   directionTileMap[dir] = tile;
   tile->directionTileMap[dir.opposite()] = spMapTile(_weak);
}

spMapTile MapTile::getTileInDirection( const Direction& dir) const
{
   map<Direction, spMapTile>::const_iterator iter;
   iter = directionTileMap.find(dir);
   if (iter != directionTileMap.end())
   {
      return iter->second;
   }
   else 
   {
      return spMapTile();
   }
}

int MapTile::getMoveCost( const Direction& dir) const
{
   int baseCost = (dir.isCardinal()) ? cardinalMoveBase : diagonalMoveBase;
   return static_cast<int>(getTileInDirection(dir)->_terrain->getMoveMultiplier() * baseCost);
}

spTerrain MapTile::getTerrain() const
{
   return _terrain;
}

void MapTile::setTerrain(spTerrain terrain)
{
   _terrain = terrain;
}

void MapTile::drawTerrain(Point offset) const
{
   throw UnsupportedOperationException();
}

//vector<string> MapTile::getResourceIDs()
//{
//   vector<string> resIDs;
//   resIDs.push_back(_terrain.getResourceID());
//}

void MapTile::takeDamage( int damage, bool explosion )
{
    if ( _unit.get() )
    {
        _unit->takeDamage( damage );
    }
    if (explosion)
    {
        list<spMapObject>::iterator iter;
        for (iter = _objects.begin(); iter != _objects.end(); ++iter)
        {
           (*iter)->takeDamage(damage);
        }
    }
    updateAdjacentWalls();
}

// hackerrific
void MapTile::updateAdjacentWalls()
{
   vector<const Direction*> dirs = Direction::getNonCardinalDirections();
   vector<const Direction*>::const_iterator iter;
   for(iter = dirs.begin(); iter != dirs.end(); ++iter)
   {
      spMapTile t = getTileInDirection(**iter);
   }
}

#include <cmath>
int MapTile::getDistance( spMapTile tile )
{
    int x = max ( _x - tile->getX(), tile->getX() - _x );
    int y = max ( _y - tile->getY(), tile->getY() - _y );
    return int(sqrt ( (double)(x*x) + (double)(y*y) ));
}

void MapTile::addDebris() { _hasDebris = true; }

bool MapTile::hasUseableObjects()
{
   list<spMapObject>::iterator iter;
   for (iter = _objects.begin(); iter != _objects.end(); ++iter)
   {
      if ((*iter)->canUse())
      {
         return true;
      }
   }
   return false;
}

void MapTile::useObjects()
{
   list<spMapObject>::iterator iter;
   for (iter = _objects.begin(); iter != _objects.end(); ++iter)
   {
      if ((*iter)->canUse())
      {
         (*iter)->use();
      }
   }
}

} // namespace
