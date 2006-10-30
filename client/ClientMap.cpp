#include "includes.h"
#include "ClientMap.h"
#include "ClientMapTile.h"
#include "Display.h"
#include "ClientEntityFactory.h"

#define TILE_MOUSE_MAP "resources/images/tile_mousemap.png"

ClientMap::ClientMap(spEntityFactory factory) :
   Map(factory)
{
   _tileMouseMap = Display::instance().loadImage(TILE_MOUSE_MAP, false);
}

ClientMap::~ClientMap()
{

}

spClientMap ClientMap::makeMap()
{
   return makeMapHelper<ClientMap>(ClientEntityFactory::instancePtr());
}

spMapTile ClientMap::getMouseOverTile() const
{
   return _mouseOverTile;
}

void ClientMap::updateMouseOverTile(const Point& mouse, const Point& offset)
{
   static Point size(ClientMapTile::getWidth(), ClientMapTile::getHeight());
   int x = mouse.x + offset.x;
   int y = mouse.y + offset.y;

   vector<spMapTile>::iterator iter;

   // clear tile, let below reset it
   _mouseOverTile = spClientMapTile();

   for (iter = _mapTiles.begin(); iter != _mapTiles.end(); ++iter)
   {
      spMapTile tile = *iter;
      Point origin(tile->getScreenX(), tile->getScreenY());
      if (x > origin.x && x < origin.x + size.x && y > origin.y && y < origin.y + size.y)
      {
         // difference from pixel to origin of image
         Point diff(x - origin.x, y - origin.y);

         if (SDL_MUSTLOCK(_tileMouseMap))
         {
            SDL_LockSurface(_tileMouseMap);
         }
 
         Uint16* pixels = static_cast<Uint16*>(_tileMouseMap->pixels);
         Uint16 pix = pixels[diff.x + diff.y*_tileMouseMap->w];

         Uint16 red = pix & _tileMouseMap->format->Rmask;
         Uint16 green = pix & _tileMouseMap->format->Gmask;
         Uint16 blue = pix & _tileMouseMap->format->Bmask;

         if (SDL_MUSTLOCK(_tileMouseMap))
         {
            SDL_UnlockSurface(_tileMouseMap);
         }
         
         // offset from current image to actual MapTile
         Point offset;

         if (red && !green && !blue) 
         {
            offset.y = -1;
         }
         else if (!red && green && !blue)
         {
            offset.x = -1;
         }
         else if (!red && !green && blue)
         {
            offset.x = 1;
         }
         else if (red && green && !blue)
         {
            offset.y = 1;
         }

         Point coords(tile->getX() + offset.x, tile->getY() + offset.y);
         if (coords.x >= 0 && coords.x < getWidth() && coords.y >= 0 && coords.y < getHeight())
         {
            _mouseOverTile = getTile(coords.x, coords.y);
            break;
         }
      }
   }
}

void ClientMap::drawTerrainLayer(const Point& offset)
{
   int height = getHeight();
   int width = getWidth();

   for (int i=0; i < height; i++)
   {
      for (int j=0; j < width; j++)
      {
         getTile(j, i)->drawTerrain(offset);
      }
   }
}

void ClientMap::highlightMouseOverTile(const Point& offset)
{
   if (_mouseOverTile.get())
   {
      _mouseOverTile->highlight(offset);
   }
}

void ClientMap::drawObjectLayer(const Point& offset)
{
   int height = getHeight();
   int width = getWidth();

   for (int i=0; i < height; i++)
   {
      for (int j=0; j < width; j++)
      {
         getTile(j, i)->drawObjects(offset);
      }
   }
}

spMapTile ClientMap::makeMapTile(TerrainType type, int x, int y)
{
   return ClientMapTile::makeMapTile(type, x, y);
}
