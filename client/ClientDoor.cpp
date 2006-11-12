#include "includes.h"
#include "ClientDoor.h"
#include "Display.h"

SDL_Surface* ClientDoor::_nwse_closed = NULL;
SDL_Surface* ClientDoor::_nwse_open = NULL;
SDL_Surface* ClientDoor::_nesw_closed = NULL;
SDL_Surface* ClientDoor::_nesw_open = NULL;

#define DOOR_IMAGE_NWSE_CLOSED "resources/images/door_nwse_closed.png"
#define DOOR_IMAGE_NWSE_OPEN   "resources/images/door_nwse_open.png"
#define DOOR_IMAGE_NESW_CLOSED "resources/images/door_nesw_closed.png"
#define DOOR_IMAGE_NESW_OPEN   "resources/images/door_nesw_open.png"

ClientDoor::ClientDoor(uint32 entityID, WALL_TYPE type, bool open) :
   Door(entityID, type, open),
   _image(NULL)
{
   static bool imagesLoaded = false;
   if (!imagesLoaded)
   {
      _nwse_closed = Display::instance().loadImage(DOOR_IMAGE_NWSE_CLOSED);
      _nwse_open = Display::instance().loadImage(DOOR_IMAGE_NWSE_OPEN);
      _nesw_closed = Display::instance().loadImage(DOOR_IMAGE_NESW_CLOSED);
      _nesw_open = Display::instance().loadImage(DOOR_IMAGE_NESW_OPEN);
      imagesLoaded = true;
   }

   updateImage();
}

ClientDoor::~ClientDoor()
{

}

void ClientDoor::use()
{
   Door::use();
   updateImage();
}

void ClientDoor::updateImage()
{
   switch (_type)
   {
      case WT_NE:
      case WT_SW:
      case WT_NE_SW:
         _image = (_open) ? _nesw_open : _nesw_closed;
         break;
      case WT_NW:
      case WT_SE:
      case WT_NW_SE:
         _image = (_open) ? _nwse_open : _nwse_closed;
         break;

      default:
         _image = _nwse_open;
         break;
   }
}

void ClientDoor::draw(Point position, Point dimensions) const
{
   Display::instance().draw(position.x, position.y - (_image->h - dimensions.y), _image);
}
