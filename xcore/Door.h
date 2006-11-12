#ifndef XCORE_DOOR_H
#define XCORE_DOOR_H

#include "includes.h"
#include "MapObject.h"
#include "Wall.h"

namespace xcore 
{

class Door;
typedef boost::shared_ptr<Door> spDoor;

/*enum DOOR_TYPE { DT_NONE = 0,
                  DT_NW = 1,
                  DT_NE = 2,
                  DT_SE = 4,
                  DT_SW = 8,
                  DT_NE_SW = 10, 
                  DT_NW_SE = 5 };*/

class Door : public MapObject
{
   public:
      Door(uint32 entityID, WALL_TYPE type, bool open=false);
      virtual ~Door();

      string getName();
      bool canUse();
      void use();

      bool isPassable() const;
      //bool isPassable(const Direction& dir) const;
      void destroy();

      void draw(Point position, Point dimensions) const;

   protected:
      WALL_TYPE _type;
      bool _open;

   private:
      bool _destroyed;
/*      static SDL_Surface* _image;
      static SDL_Surface* _imageNE;
      static SDL_Surface* _imageNW;
      static SDL_Surface* _imageSE;
      static SDL_Surface* _imageSW;
*/
};

} // namespace

#endif
