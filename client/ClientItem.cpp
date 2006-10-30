#include "includes.h"
#include "ClientItem.h"

ClientItem::ClientItem(uint32 entityID, spItem item) :
   Item(entityID),
   _item(item),
   _image(NULL),
   _invImage(NULL)
{
   _name = _item->getName();
   _effectiveRange = _item->getRange();
   _type = _item->getType();
   _turnCost = _item->getTurnCost();
}

ClientItem::~ClientItem()
{
}

SDL_Surface* ClientItem::getImage()
{
   return _image;
}
SDL_Surface* ClientItem::getInvImage()
{
   return _invImage;
}

