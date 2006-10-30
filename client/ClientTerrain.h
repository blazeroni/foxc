#ifndef XCLIENT_CLIENTTERRAIN_H
#define XCLIENT_CLIENTTERRAIN_H

#include "includes.h"
#include "xcore/Terrain.h"

class ClientTerrain;
typedef boost::shared_ptr<ClientTerrain> spClientTerrain;

class ClientTerrain : public Terrain
{
   public:
      ClientTerrain(TerrainType type);
      virtual ~ClientTerrain();

      virtual void draw(int x, int y) const;

   private:
      static map<TerrainType, SDL_Surface*> _imageMap;
};

#endif
