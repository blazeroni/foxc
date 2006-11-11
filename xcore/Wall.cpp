#include "includes.h"
#include "Wall.h"
#include "UnsupportedOperationException.h"
//#include "Display.h"

/*
#define WALL_IMAGE "images/wall.png"
#define WALL_IMAGENE "images/wall-ne.png"
#define WALL_IMAGENW "images/wall-nw.png"
#define WALL_IMAGESE "images/wall-se.png"
#define WALL_IMAGESW "images/wall-sw.png"

SDL_Surface* Wall::_image = NULL;
SDL_Surface* Wall::_imageNE = NULL;
SDL_Surface* Wall::_imageNW = NULL;
SDL_Surface* Wall::_imageSE = NULL;
SDL_Surface* Wall::_imageSW = NULL;
*/

namespace xcore
{

Wall::Wall(uint32 entityID, WALL_TYPE type) :
   MapObject(entityID),
   _type(type),
   _destroyed(false)
{
   //if (!_image)
   //{
   //   _image = Display::instance().loadImage(WALL_IMAGE);
   //   _imageNE = Display::instance().loadImage(WALL_IMAGENE);
   //   _imageNW = Display::instance().loadImage(WALL_IMAGENW);
   //   _imageSE = Display::instance().loadImage(WALL_IMAGESE);
   //   _imageSW = Display::instance().loadImage(WALL_IMAGESW);
   //}
}

Wall::~Wall()
{
}

bool Wall::isPassable() const
{
    return _destroyed;
}

void Wall::destroy()
{
   // @ todo fill in later
}

void Wall::draw(Point position, Point dimensions) const
{
   throw UnsupportedOperationException();
    // @ if not destroyed??
   //Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _image);
   /*
   switch ( _dir )
   {
       case NE:
            Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageNE);
            break;
       case NW:
            Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageNW);
            break;
       case SE:
            Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageSE);
            break;
       case SW:
            Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageSW);
            break;

   }
   */
    //if ( _dir == Direction::NE )
    //        Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageNE);
    //else if ( _dir == Direction::NW )
    //        Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageNW);
    //else if ( _dir == Direction::SE )
    //        Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageSE);
    //else if ( _dir == Direction::SW )
    //        Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _imageSW);
}

} // namespace
