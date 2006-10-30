#ifndef XCLIENT_CLIENTMAPTILE_H
#define XCLIENT_CLIENTMAPTILE_H

#include "includes.h"
#include "xcore/MapTile.h"
#include "IDisplayable.h"
#include "ClientUnit.h"

class ClientMapTile;
typedef boost::shared_ptr<ClientMapTile> spClientMapTile;

class ClientMapTile : public MapTile
{
   // to allow MapTile factory method to create these objects
   friend class MapTile;

   public:
      virtual ~ClientMapTile();

      static spClientMapTile makeMapTile(TerrainType type, int x, int y);

      virtual spTerrain makeTerrain(TerrainType terrain);

      spClientUnit getUnit() const;    // explicitly non-virtual

      virtual void drawTerrain(Point offset) const;
      virtual void drawObjects(Point offset) const;
      virtual void highlight(Point offset);
      virtual void highlightMoveable(Point offset);

      virtual int getScreenX() const;
      virtual int getScreenY() const;
      virtual int getCenterX() const;
      virtual int getCenterY() const;

      static int getWidth();
      static int getHeight();

   protected: 
      ClientMapTile(TerrainType type, int x, int y);

   private:
      int _screenX;
      int _screenY;
      int _centerX;
      int _centerY;
      static int _width;
      static int _height;
      static SDL_Surface *_highlight, *_highlightMove, *_highlightFog;
};

#endif
