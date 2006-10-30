#ifndef XCORE_TERRAIN_H
#define XCORE_TERRAIN_H

#include "includes.h"
#include "Point.h"

namespace xcore
{

enum TerrainType { GRASS, WATER };

#define IMPASSABLE_TERRAIN_MULTIPLIER 1000.0
#define GRASS_TILE_RESOURCE_ID "tile_grass"
#define WATER_TILE_RESOURCE_ID "tile_water"

class Terrain;
typedef boost::shared_ptr<Terrain> spTerrain;

class Terrain
{
   public:
      Terrain(TerrainType type);
      virtual ~Terrain();

      bool isPassable() const;
      TerrainType getType() const;
      double getMoveMultiplier() const;
      virtual void draw(int x, int y) const;

      string getResourceID();
 
   private:
      class TerrainBaseType;

      TerrainBaseType* _type;
      static map<TerrainType, TerrainBaseType*> _possibleTypes;
      static int _refCount;

      // inner classes for dealing with TerrainTypes
      class TerrainBaseType
      {
         public:
            virtual ~TerrainBaseType() {}
            virtual bool isPassable() = 0;
            virtual TerrainType getType() = 0;
            virtual double getMoveMultiplier() = 0;
            virtual string getResourceID() = 0;
      };

      class TerrainGrass : public TerrainBaseType
      {
         public: 
            virtual bool isPassable() { return true; }
            virtual TerrainType getType() { return GRASS; }
            virtual double getMoveMultiplier() { return 1; }
            virtual string getResourceID() { return GRASS_TILE_RESOURCE_ID; }
      };

      class TerrainWater : public TerrainBaseType
      {
         public:
            virtual bool isPassable() { return false; }
            virtual TerrainType getType() { return WATER; }
            virtual double getMoveMultiplier() { return IMPASSABLE_TERRAIN_MULTIPLIER; }
            virtual string getResourceID() { return WATER_TILE_RESOURCE_ID; }
      };
};

} // namespace 

#endif
