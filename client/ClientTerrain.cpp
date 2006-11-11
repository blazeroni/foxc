#include "includes.h"
#include "ClientTerrain.h"
#include "Display.h"

#define GRASS_IMAGE "resources/images/tile_grass.png"
#define WATER_IMAGE "resources/images/tile_water.png"
#define FLOOR_IMAGE "resources/images/tile_floor.png"

map<TerrainType, SDL_Surface*> ClientTerrain::_imageMap;

ClientTerrain::ClientTerrain(TerrainType type) :
   Terrain(type)
{
   static bool mapped = false;
   if (!mapped) 
   {
      // this is currently a memory leak...
      _imageMap[GRASS] = Display::instance().loadImage(GRASS_IMAGE);
      _imageMap[WATER] = Display::instance().loadImage(WATER_IMAGE);
	  _imageMap[FLOOR] = Display::instance().loadImage(FLOOR_IMAGE);

      mapped = true;
   }
}

ClientTerrain::~ClientTerrain()
{

}

void ClientTerrain::draw(int x, int y) const
{
   Display::instance().draw(x, y, _imageMap[getType()]);
}
