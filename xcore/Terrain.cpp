#include "includes.h"
#include "Terrain.h"
#include "UnsupportedOperationException.h"
//#include "Display.h"

//#define GRASS_IMAGE "images/tile_grass.png"
//#define WALL_IMAGE "tile_wall.png"
//#define WATER_IMAGE "images/tile_water.png"

namespace xcore
{

map<TerrainType, Terrain::TerrainBaseType*> Terrain::_possibleTypes;
//map<TerrainType, SDL_Surface*> Terrain::_imageMap;
int Terrain::_refCount = 0;

Terrain::Terrain(TerrainType type)
{
   // reference counted
   if (_refCount == 0)
   {
      _possibleTypes[GRASS] = new TerrainGrass();
      _possibleTypes[WATER] = new TerrainWater();
	  _possibleTypes[FLOOR] = new TerrainFloor();
	  _possibleTypes[SNOWS] = new TerrainSnows();
   }

   _type = _possibleTypes[type];
   _refCount++;
}

Terrain::~Terrain()
{
   _refCount--;
   if (_refCount == 0)
   {
      map<TerrainType, TerrainBaseType*>::iterator iter;
      for (iter = _possibleTypes.begin(); iter != _possibleTypes.end(); ++iter)
      {
         SAFE_DELETE((*iter).second);
      }
   }
}

bool Terrain::isPassable() const
{
   return _type->isPassable();
}

TerrainType Terrain::getType() const
{
   return _type->getType();
}

double Terrain::getMoveMultiplier() const
{
   return _type->getMoveMultiplier();
}

void Terrain::draw(int x, int y) const
{
   throw UnsupportedOperationException();
}

} //namespace
