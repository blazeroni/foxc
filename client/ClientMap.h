#ifndef XCLIENT_CLIENTMAP_H
#define XCLIENT_CLIENTMAP_H

#include "includes.h"
#include "xcore/Map.h"
#include "ClientMapTile.h"

class ClientMap;
typedef boost::shared_ptr<ClientMap> spClientMap;

class ClientMap : public Map
{
   // to allow creation via factory method
   friend class Map;

   public:
      static spClientMap makeMap();
      virtual ~ClientMap();

      //spClientMapTile getTile(int x, int y) const;    // explicitly non-virtual

      virtual spMapTile getMouseOverTile() const;
      virtual void updateMouseOverTile(const Point& mouse, const Point& offset);

      virtual void drawTerrainLayer(const Point& offset);
      virtual void drawObjectLayer(const Point& offset);

      virtual void highlightMouseOverTile(const Point& offset);

   protected:
      ClientMap(spEntityFactory factory);

      virtual spMapTile makeMapTile(TerrainType type, int x, int y);

   private:
      spMapTile _mouseOverTile;
      SDL_Surface* _tileMouseMap;
};

#endif
